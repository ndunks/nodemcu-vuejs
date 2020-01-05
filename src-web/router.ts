import Vue from 'vue'
import VueRouter from 'vue-router'
import Dashboard from '@/views/Dashboard.vue'
import About from '@/views/About.vue'
import Login from '@/views/Login.vue'
import store from '@/store'
import { Popup } from '@/interfaces'

Vue.use(VueRouter)

const routes = [
  {
    path: '/',
    component: Login,
    meta: {
      title: 'Access System',
      public: true,
      hideMenu: true // Dont show in sidebar menu
    }
  },
  {
    path: '/dash',
    component: Dashboard,
    meta: {
      title: "",
      label: "Dashboard",
      icon: 'home',
    }
  },
  {
    path: '/about',
    component: About,
    meta: {
      title: 'About Device',
      label: 'About', // text displayed on sidebar menu
      icon: 'help',
      public: true
    }
  }
]

const router = new VueRouter({
  mode: 'history',
  base: process.env.BASE_URL,
  routes
})

router.afterEach((to, from) => {
  if (to.meta.title) {
    // Auto set router title
    store.commit('title', to.meta.title);
  }
})

router.beforeEach(async (to, from, next) => {
  if (!store.state.bootComplete) {
    // Waiting boot complete
    await new Promise((r, j) => {
      const unwatch = store.watch(
        (s) => s.bootComplete,
        (v) => {
          r();
          unwatch()
        }
      )
    })
  }
  let nextPath: any = undefined
  if (store.state.login) {
    if (to.meta.isLoginPage) {
      nextPath = '/dash'
    }
  } else if (!to.meta.public) {
    store.commit('popup', {
      color: 'warning',
      message: 'Please login first'
    } as Popup)
    // check allowed path
    let next = to.path;

    if (routes.findIndex(v => v.path == next) < 0) {
      next = '/dash'
    }

    nextPath = {
      path: '/',
      query: { next }
    }
  }
  return next(nextPath)
})
export default router
