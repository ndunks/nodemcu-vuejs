import { Popup } from '@/interfaces';

const state: {
    bootComplete: boolean
    login: boolean
    title: string
    popups: Popup[]
} = {
    bootComplete: false,
    login: false,
    title: 'Smart Device..',
    popups: []
}

export default state
