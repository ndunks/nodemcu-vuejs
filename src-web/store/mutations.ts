import { Store } from 'vuex'
import { Popup, StatusRaw, WifiMode, WifiStatus, ConfigRaw, isConfigRaw } from '@/interfaces';

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
    },
    status(state, payload: StatusRaw | ConfigRaw) {
        const [free, max, frag] = payload.heap.split(' ', 3)
        state.status.heap = {
            free: parseInt(free),
            max: parseInt(max),
            frag: parseInt(frag)
        }
        state.status.status = parseInt(payload.status)
        state.status.statusStr = WifiStatus[state.status.status]
        state.status.rssi = parseInt(payload.rssi)
        state.status.signal = 100 + parseInt(payload.rssi)
        state.status.ap_clients = parseInt(payload.ap_clients)

        if (!isConfigRaw(payload))
            return;

        state.status.mode = parseInt(payload.mode)
        state.status.modeStr = WifiMode[state.status.mode]
        state.status.hostname = payload.hostname
        state.status.autoconnect = parseInt(payload.autoconnect) > 0
        state.status.ssid = payload.ssid
        state.status.ip = payload.ip
        state.status.gateway = payload.gateway
        state.status.ap_ssid = payload.ap_ssid
        state.status.ap_ip = payload.ap_ip
        state.status.ap_psk = payload.ap_psk
        //console.log(state.status.mode, WifiMode["Access Point"], state.status.mode & WifiMode["Access Point"])
        state.status.isApMode = !!(state.status.mode & WifiMode["Access Point"])
        state.status.isStaMode = !!(state.status.mode & WifiMode.Station)
    }
}
export default mutations;