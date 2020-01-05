<template>
  <v-card>
    <v-toolbar short flat>
      <v-toolbar-title>
        WiFi
        <span class="subtitle-1">
          Mode: {{ status.modeStr }}
        </span>
      </v-toolbar-title>
      <v-spacer />
      <v-dialog max-width="500" ref="dialog">
        <template #activator="{on}">
          <v-btn v-on="on" @click="scan" text color="primary">
            Scan WiFi
          </v-btn>
        </template>
        <v-card>
          <v-progress-linear absolute v-if="loading" indeterminate />
          <v-toolbar short flat>
            <v-toolbar-title>Available WiFi Networks</v-toolbar-title>
            <v-spacer />
            <v-btn @click="scan" :disabled="loading" text icon color="primary">
              <v-icon>refresh</v-icon>
            </v-btn>
          </v-toolbar>
          <v-list-item v-for="wifi in wifiList" :key="wifi.ssid">
            <v-list-item-avatar>
              <v-progress-circular :value="wifi.signal" color="primary">
                {{ wifi.signal }}
              </v-progress-circular>
            </v-list-item-avatar>
            <v-list-item-content>
              <v-list-item-title v-text="wifi.ssid" />
              <v-list-item-subtitle v-text="wifi.security" />
            </v-list-item-content>
            <v-list-item-action>
              <v-btn
                v-if="status.connected && status.ssid == wifi.ssid"
                @click="disconnect(wifi)"
                text
                color="error"
              >
                Disconnect
              </v-btn>
              <v-btn v-else text color="success" @click="connect(wifi)">
                Connect
              </v-btn>
            </v-list-item-action>
          </v-list-item>
        </v-card>
      </v-dialog>
    </v-toolbar>

    <v-subheader>Station Info</v-subheader>
    <template v-if="status.isStaMode">
      <v-list-item>
        <v-list-item-content>
          <v-list-item-title>SSID</v-list-item-title>
          <v-list-item-subtitle>
            {{ status.ssid }}
          </v-list-item-subtitle>
        </v-list-item-content>
      </v-list-item>
      <v-list-item>
        <v-list-item-content>
          <v-list-item-title>Status</v-list-item-title>
          <v-list-item-subtitle>
            {{ status.statusStr }}
          </v-list-item-subtitle>
        </v-list-item-content>
      </v-list-item>
      <v-list-item v-if="status.isConnected">
        <v-list-item-content>
          <v-list-item-title>Signal</v-list-item-title>
          <v-list-item-subtitle>
            <v-progress-circular :value="status.signal" color="primary">
              {{ status.signal }}
            </v-progress-circular>
          </v-list-item-subtitle>
        </v-list-item-content>
      </v-list-item>
      <v-list-item>
        <v-list-item-content>
          <v-list-item-title>IP</v-list-item-title>
          <v-list-item-subtitle>
            {{ status.ip }}
          </v-list-item-subtitle>
        </v-list-item-content>
      </v-list-item>
      <v-list-item>
        <v-list-item-content>
          <v-list-item-title>Gateway</v-list-item-title>
          <v-list-item-subtitle>
            {{ status.gateway }}
          </v-list-item-subtitle>
        </v-list-item-content>
      </v-list-item>
    </template>
    <v-list-item v-else @click="$refs.dialog.isActive = true">
      <v-list-item-content>
        <v-list-item-title>Connect to WiFi</v-list-item-title>
      </v-list-item-content>
    </v-list-item>

    <v-subheader>Access Point Info</v-subheader>
    <template v-if="status.isApMode">
      <v-list-item>
        <v-list-item-content>
          <v-list-item-title>SSID</v-list-item-title>
          <v-list-item-subtitle>
            {{ status.ap_ssid }}
          </v-list-item-subtitle>
        </v-list-item-content>
      </v-list-item>
      <v-list-item>
        <v-list-item-content>
          <v-list-item-title>IP</v-list-item-title>
          <v-list-item-subtitle>
            {{ status.ap_ip }}
          </v-list-item-subtitle>
        </v-list-item-content>
      </v-list-item>
      <v-list-item>
        <v-list-item-content>
          <v-list-item-title>Password</v-list-item-title>
          <v-list-item-subtitle>
            {{ status.ap_psk || "(No Password)" }}
          </v-list-item-subtitle>
        </v-list-item-content>
      </v-list-item>
      <v-list-item>
        <v-list-item-content>
          <v-list-item-title>Connected Clients</v-list-item-title>
          <v-list-item-subtitle>
            {{ status.ap_clients }}
          </v-list-item-subtitle>
        </v-list-item-content>
      </v-list-item>
    </template>
    <v-list-item v-else>
      <v-list-item-content>
        <v-list-item-title>Enable Access Point</v-list-item-title>
      </v-list-item-content>
    </v-list-item>
  </v-card>
</template>
<script lang="ts">
import Vue from 'vue'
import Component from 'vue-class-component';
import { Popup, Status, WifiMode } from '../interfaces';
import Api from '../api';
import { mapState } from 'vuex';

@Component({
  computed: mapState(['loading', 'status'])
})
export default class WidgetWifi extends Vue {
  wifiList: {
    id: number
    ssid: string
    security: string
    signal: number
  }[] = []

  // Typing helper
  status: Status

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
              signal: 100 + parseInt(tmp[0]),
              security: tmp[1],
              ssid: tmp[2]
            })
          }
        )
      }
    )
  }

  connect(wifi) {

  }
  disconnect(wifi) {

  }
}
</script>
