import { GetterTree } from 'vuex'

const getters: GetterTree<State, any> = {
    loading() {
        ///@ts-ignore
        return window.loading.counter > 0
    }
}

export default getters;