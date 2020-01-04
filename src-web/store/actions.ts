import { ActionTree } from 'vuex'
import Api from '@/api'

const actions: ActionTree<State, any> = {
    async boot(context) {
        // Delay test
        //await new Promise((r, j) => setTimeout( r, 3000))
        // Validating password
        if (Api.password) {
            await Api.get('ping').then(
                resp => context.commit('login', true)
            ).catch(
                e => Api.password = null
            )
        }
        context.commit('bootComplete')
    }

}
export default actions;