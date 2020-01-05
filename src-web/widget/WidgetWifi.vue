<template>
  <v-card>
    <v-card-title>
      WiFi Mode:
      <span class="subtitle-1">{{ status.modeStr }} </span>
    </v-card-title>
    <v-subheader>Station Info</v-subheader>
    <template v-if="isStaMode">
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
        <v-list-item-action>
          <v-btn color="error" text @click="disconnect">
            Disconnect
          </v-btn>
        </v-list-item-action>
      </v-list-item>
      <v-list-item v-if="isConnected">
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
    <v-dialog v-else>
      <template #activator="{on}">
        <v-list-item v-on="on">
          <v-list-item-content>
            <v-list-item-title>Connect to WiFi</v-list-item-title>
          </v-list-item-content>
        </v-list-item>
      </template>
      <WidgetConnect />
    </v-dialog>

    <v-subheader>Access Point Info</v-subheader>
    <template v-if="isApMode">
      <v-list-item>
        <v-list-item-content>
          <v-list-item-title>SSID</v-list-item-title>
          <v-list-item-subtitle>
            {{ status.ap_ssid }}
          </v-list-item-subtitle>
        </v-list-item-content>
        <v-list-item-action>
          <v-btn color="error" text @click="setApMode(false)">
            Disable
          </v-btn>
        </v-list-item-action>
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
    <v-list-item v-else @click="setApMode(true)">
      <v-list-item-content>
        <v-list-item-title class="success--text"
          >Disabled (Click to Enable)</v-list-item-title
        >
      </v-list-item-content>
    </v-list-item>
  </v-card>
</template>
<script lang="ts">
import Vue from 'vue'
import Component from 'vue-class-component';
import { Status } from '@/interfaces';
import { mapState } from 'vuex';
import WidgetConnect from "./WidgetConnect.vue";
import Api from '../api';

@Component({
  components: { WidgetConnect },
  computed: mapState(['loading', 'status'])
})
export default class WidgetWifi extends Vue {

  // Typing helper
  status: Status

  get isStaMode(): boolean {
    return this.status.isStaMode
  }
  get isConnected(): boolean {
    return this.status.isConnected
  }
  get isApMode(): boolean {
    return this.status.isApMode
  }
  /** Disconect STA */
  disconnect() {
    if (!confirm("Are you sure? you may lost connection to this device.")) {
      return;
    }
    Api.get('wifi', {
      params: {
        disconnect: true
      }
    }).then(response => this.$store.dispatch('status'))
  }
  /** Disable AP Mode */
  setApMode(enable: boolean) {
    if (!enable && !confirm("Are you sure? you may lost connection to this device.")) {
      return;
    }
    Api.get('wifi', {
      params: {
        ap: enable
      }
    }).then(response => this.$store.dispatch('status'))
  }
}
</script>
