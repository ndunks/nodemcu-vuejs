import '@/style.scss'
import '@/plugin'
import Vue from 'vue'
import router from "@/router";
import store from "@/store";
import App from './App.vue'
import Vuetify from 'vuetify/lib'

Vue.config.productionTip = false

const vuetify = new Vuetify({
  icons: {
    iconfont: 'md',
  },
});

new Vue({
  router,
  store,
  vuetify,
  render: h => h(App)
} as any).$mount('#app')

store.dispatch('boot')