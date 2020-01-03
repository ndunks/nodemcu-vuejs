import Vue from 'vue'
import App from './App.vue'
import router from './router'
import Vuetify from 'vuetify/lib'
import '@/style.css'

Vue.config.productionTip = false

Vue.use(Vuetify);

const vuetify = new Vuetify({
  icons: {
    iconfont: 'md',
  },
});

new Vue({
  router,
  ///@ts-ignore
  vuetify,
  render: h => h(App)
}).$mount('#app')
