import { Store } from 'vuex'
import { Popup } from '@/interfaces';

const mutations: {
    [name: string]: (this: Store<State>, state: State, payload?: any) => any
} = {
    loading(state, loading: boolean) {
        ///@ts-ignore
        state.loading = (window.loading.counter += loading ? 1 : -1) > 0;
    },
    login(state, login: boolean) {
        state.login = login
    },
    bootComplete(state) {
        state.bootComplete = true
        //Default loading is 1 on page load, hide it now
        this.commit('loading', false);
    },
    title(state, payload: string) {
        state.title = payload
        document.title = payload
    },
    popup(state, payload: Popup | string) {
        if (typeof payload != 'object') {
            payload = {
                color: 'warning',
                message: payload
            } as Popup
        }
        payload.id = Math.round(Math.random() * 100000).toString(32) + Date.now().toString(32)
        Object.defineProperty(payload, 'state', {
            get() {
                return true
            },
            set: (v) => {
                this.commit('removePopup', payload)
            }
        })
        state.popups.push(payload)
    },
    removePopup(state, payload: Popup) {
        const index = state.popups.indexOf(payload)
        if (index >= 0) {
            state.popups.splice(index, 1)
        }
    }
}
export default mutations;