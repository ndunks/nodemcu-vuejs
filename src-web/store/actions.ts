import { ActionTree } from 'vuex'
import Api from '@/api'

const actions: ActionTree<State, any> = {
    async boot(context) {
        // Delay test
        //await new Promise((r, j) => setTimeout( r, 3000))
        let password = localStorage && localStorage.getItem('password');
        if (password) {
            Api.password = password;
            await context.dispatch('ping').catch(
                e => {
                    Api.password = null
                    localStorage.removeItem('password')
                }
            )
        }
        context.commit('bootComplete')
    }

}
export default actions;