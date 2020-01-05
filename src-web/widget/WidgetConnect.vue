<template>
  <v-card>
    <v-progress-linear absolute v-if="loading" indeterminate />
    <v-toolbar short flat>
      <v-toolbar-title>Available WiFi Networks</v-toolbar-title>
      <v-spacer />
      <v-btn @click="scan" :disabled="loading" text icon color="primary">
        <v-icon>refresh</v-icon>
      </v-btn>
    </v-toolbar>
    <v-list-item v-for="wifi in wifiList" :key="wifi.ssid" :disabled="loading">
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
          v-if="status.isConnected && status.ssid == wifi.ssid"
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
    <DialogConfirm
      :message="dialogMessage"
      :title="dialogTitle"
      :input-message="dialogInput"
      v-model="dialogActions"
    />
  </v-card>
</template>
<script lang="ts">
import Vue from 'vue'
import Component from 'vue-class-component';
import { Wifi, Status, ActionDialog } from '@/interfaces';
import Api from '@/api';
import { mapState } from 'vuex';
import DialogConfirm from "@/dialog/DialogConfirm.vue";

@Component({
  computed: mapState(['loading', 'status'])
})
export default class WidgetConnect extends Vue {
  dialogTitle: string = null
  dialogInput: string = null
  dialogMessage: string = null
  dialogActions: ActionDialog[] = null
  wifiList: Wifi[] = []
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

  connect(wifi: Wifi) {
    const action = async (pass?: string) => {
      await Api.get('wifi', {
        params: {
          connect: wifi.id,
          pass
        }
      })
      this.$store.dispatch('status')
      this.$emit('done');
    }

    if (!this.status.isConnected) {
      return action()
    }
    if (wifi.security != 'none') {
      this.dialogInput = "Wifi password"
    } else {
      this.dialogInput = null
    }
    this.dialogTitle = `Connect to ${wifi.ssid}?`
    this.dialogMessage = "Device IP may be changed and You may lost connection."
    this.dialogActions = [
      {
        label: 'Cancel',
        color: 'success'
      },
      'spacer',
      {
        label: `OK, Connect to ${wifi.ssid}`,
        color: "error",
        action
      }
    ]
  }

  disconnect(wifi: Wifi) {
    this.dialogInput = null
    this.dialogTitle = `Disconnect from ${wifi.ssid}?`
    this.dialogMessage = "You may lost connection to this device."
    this.dialogActions = [
      {
        label: 'Cancel',
        color: 'success'
      },
      'spacer',
      {
        label: "OK, Disconnect it",
        color: "error",
        async action() {
          await Api.get('wifi', { params: { disconnect: true } })
          this.$store.dispatch('status')
          this.$emit('done');
        }
      }
    ]
  }
  mounted() {
    this.scan()
  }
}
</script>
