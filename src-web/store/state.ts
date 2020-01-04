import { Popup } from '@/interfaces';

const state: {
    bootComplete: boolean
    login: boolean
    loading: boolean
    title: string
    popups: Popup[]
} = {
    bootComplete: false,
    login: false,
    loading: false,
    title: 'Smart Device..',
    popups: []
}

export default state
