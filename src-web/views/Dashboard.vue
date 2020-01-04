<template>
  <v-container class="fill-height" fluid>
    <v-row align="center" justify="center">
      <v-col class="text-center">
        <v-card class="mx-auto">
          <v-toolbar>
            <v-toolbar-title>Wifi List</v-toolbar-title>
            <v-spacer />
            <v-btn @click="scan" :disabled="loading" text icon color="primary">
              <v-icon>refresh</v-icon>
            </v-btn>
          </v-toolbar>
          <v-list-item v-for="wifi in wifiList" :key="wifi.ssid">
            <v-list-item-content>
              <v-list-item-title v-text="wifi.ssid" />
              <v-list-item-subtitle v-text="wifi.security" />
            </v-list-item-content>
          </v-list-item>
        </v-card>
      </v-col>
    </v-row>
  </v-container>
</template>
<script lang="ts">
import Vue from 'vue'
import Component from 'vue-class-component';
import { Popup } from '../interfaces';
import Api from '../api';
import { mapState } from 'vuex';

@Component({
  computed: mapState(['loading'])
})
export default class Login extends Vue {
  wifiList: {
    id: number
    ssid: string
    security: string
    signal: number
  }[] = []
  scan() {
    Api.get("scan").then(
      response => {
        this.wifiList.splice(0)
        const data = response.data as string;
        let tmp;
        if (!data || !data.length) return;
        data.split("\n").filter(v => !!v).forEach(
          (v, i) => {
            // RSSI Security  SSID
            tmp = v.split("\t");
            this.wifiList.push({
              id: i,
              signal: 100 - parseInt(tmp[0]),
              security: tmp[1],
              ssid: tmp[2]
            })
          }
        )
      }
    )
  }
  created() {
    this.scan()
  }
}
</script>
