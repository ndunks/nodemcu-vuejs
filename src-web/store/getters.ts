import { GetterTree } from 'vuex'


const getters: GetterTree<State, any> = {
    login(state) {
        return state.login
    },
    status(state) {
        return state.status
    },
    loading(state) {
        return state.loading
    },
}

export default getters;