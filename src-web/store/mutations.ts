import { Store } from 'vuex'
import { Popup, StatusRaw, WifiMode, WifiStatus, ConfigRaw, isConfigRaw } from '@/interfaces';
import { parseResponse } from '@/helper';

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
    status(state, payload: string) {
        const raw = parseResponse<StatusRaw | ConfigRaw>(payload);
        const newStatus = { ...state.status };

        const [free, max, frag] = raw.heap.split(' ', 3)
        newStatus.heap = {
            free: parseInt(free),
            max: parseInt(max),
            frag: parseInt(frag)
        }
        newStatus.mode = parseInt(raw.mode)
        newStatus.modeStr = WifiMode[newStatus.mode]
        newStatus.status = parseInt(raw.status)
        newStatus.statusStr = WifiStatus[newStatus.status]
        newStatus.rssi = parseInt(raw.rssi)
        newStatus.signal = 100 + parseInt(raw.rssi)
        newStatus.ap_clients = parseInt(raw.ap_clients)
        newStatus.isConnected = newStatus.status == WifiStatus.Connected

        if (isConfigRaw(raw)) {
            newStatus.hostname = raw.hostname
            newStatus.autoconnect = parseInt(raw.autoconnect) > 0
            newStatus.ssid = raw.ssid
            newStatus.ip = raw.ip
            newStatus.gateway = raw.gateway
            newStatus.ap_ssid = raw.ap_ssid
            newStatus.ap_ip = raw.ap_ip
            newStatus.ap_psk = raw.ap_psk
            newStatus.isApMode = !!(newStatus.mode & WifiMode["Access Point"])
            newStatus.isStaMode = !!(newStatus.mode & WifiMode.Station)
        }
        this.state.status = newStatus;
    }
}
export default mutations;