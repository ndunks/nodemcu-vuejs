import Vue from "vue";

declare global {
    var loading: {
        counter: number
    }

    /** .env API Endpoint URL */
    var VUE_APP_API: string

    /** Globalize store state type */
    type State = typeof import('@/store/state').default
}


declare module 'vue/types/vue' {
    // Global properties can be declared
    // on the `VueConstructor` interface
    interface Vue {
    }
}