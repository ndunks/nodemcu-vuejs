import Axios, { AxiosInstance } from "axios"
import store from '@/store';

type ApiAxios = AxiosInstance & {
    /** Get or Set Authorization password */
    password?: string
    /**
     * Get absolute URL based on baseURL from relative path
     */
    getUrl?(path: string): string
}

const Api: ApiAxios = Axios.create({
    baseURL: process.env.VUE_APP_API || '/'
})

Object.defineProperty(Api, 'password', {
    get() {
        return Api.defaults.headers.Authorization
    },
    set(value: string) {
        if (value) {
            Api.defaults.headers.Authorization = value
        } else {
            delete Api.defaults.headers.Authorization;
        }
    }
})

// Fix instance getUri https://github.com/axios/axios/issues/2468
// PR not being merged: https://github.com/Alanscut/axios/commit/e8f54ad115fb63ae482c18951095fa7496d57501
Api.getUrl = function (this: ApiAxios, path: string) {
    return this.defaults.baseURL.replace(/\/+$/, '') + '/' + path.replace(/^\/+/, '')
};


function ApiErrorHandler(error) {
    store.commit('loading', false);
    store.commit('popup', {
        message: error.message,
        type: 'error'
    })
    // Error consumed here
    return Promise.reject(error)
}
Api.interceptors.request.use(
    config => {
        store.commit('loading', true);
        return config
    },
    ApiErrorHandler
)
Api.interceptors.response.use(
    response => {
        store.commit('loading', false)
        return response
    },
    ApiErrorHandler
)
export default Api