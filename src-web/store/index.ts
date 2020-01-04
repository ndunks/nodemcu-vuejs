import Vuex from 'vuex'
import state from "./state";
import actions from "./actions";
import getters from "./getters";
import mutations from "./mutations";

const store = new Vuex.Store<State>({
    state,
    getters,
    mutations,
    actions
})

export default store;