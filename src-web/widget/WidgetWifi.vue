<template>
  <v-card>
    <v-card-title>
      WiFi Mode:
      <span class="subtitle-1">{{ status.modeStr }} </span>
    </v-card-title>
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
        <v-list-item-action>
          <v-btn
            color="error"
            v-if="status.isConnected"
            text
            @click="setStaMode(false)"
          >
            Disconnect
          </v-btn>
        </v-list-item-action>
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
    <v-dialog v-else max-width="500" ref="connectDialog">
      <template #activator="{on}">
        <v-list-item v-on="on">
          <v-list-item-content>
            <v-list-item-title>Connect to WiFi</v-list-item-title>
          </v-list-item-content>
        </v-list-item>
      </template>
      <WidgetConnect @done="$refs.connectDialog.isActive = false" />
    </v-dialog>

    <v-subheader>Access Point Info</v-subheader>
    <template v-if="status.isApMode">
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
    <DialogConfirm :message="dialogMessage" v-model="dialogActions" />
  </v-card>
</template>
<script lang="ts">
import Vue from 'vue'
import Component from 'vue-class-component';
import { Status, ActionDialog } from '@/interfaces';
import { mapState } from 'vuex';
import WidgetConnect from "./WidgetConnect.vue";
import DialogConfirm from "@/dialog/DialogConfirm.vue";
import Api from '@/api';

@Component({
  components: { WidgetConnect, DialogConfirm },
  computed: mapState(['loading', 'status'])
})
export default class WidgetWifi extends Vue {
  dialogMessage: string = null
  dialogActions: ActionDialog[] = null
  // Typing helper
  status: Status

  /** Disconect STA */
  setStaMode(enable: boolean) {
    const action = () => {
      Api.get('wifi', {
        params: {
          sta: true
        }
      }).then(response => this.$store.dispatch('status'))
    }
    if (enable) {
      // No confirmation
      return action();
    }

    this.dialogMessage = "You may lost connection to this device.";
    this.dialogActions = [
      {
        label: 'Cancel',
        color: 'success'
      },
      'spacer',
      {
        label: "OK, Disconnect it",
        color: "error",
        action
      }
    ]
  }
  /** Disable AP Mode */
  setApMode(enable: boolean) {
    const action = () => {
      Api.get('wifi', {
        params: {
          ap: enable
        }
      }).then(response => this.$store.dispatch('status'))
    }
    if (enable) {
      // No confirmation
      return action();
    }

    this.dialogMessage = "You may lost connection to this device.";
    this.dialogActions = [
      {
        label: 'Cancel',
        color: 'success'
      },
      'spacer',
      {
        label: "OK, Disable it",
        color: "error",
        action
      }
    ]
  }
}
</script>
