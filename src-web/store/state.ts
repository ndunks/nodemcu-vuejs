import { Popup, Status } from '@/interfaces';

const state: {
    bootComplete: boolean
    login: boolean
    loading: boolean
    title: string
    popups: Popup[]
    status: Status
} = {
    bootComplete: false,
    login: false,
    loading: false,
    title: 'Smart Device..',
    popups: [],
    status: {} as any
}

export default state
