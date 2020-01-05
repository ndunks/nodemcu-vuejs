import { ActionTree } from 'vuex'
import Api from '@/api'
import { parseResponse } from '@/helper'

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
        // Load full status
        await Api.get("config").then(
            response => {
                context.commit('status', parseResponse(response.data))
            }
        )
        context.commit('bootComplete')
    },
    status(context) {
        Api.get("status").then(
            response => {
                context.commit('status', parseResponse(response.data))
            }
        )
    }

}
export default actions;