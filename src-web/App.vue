
<template>
  <v-app>
    <v-navigation-drawer v-model="drawer" app>
      <v-list dense>
        <v-list-item v-for="item in menuList" :key="item.path" :to="item.path">
          <v-list-item-action>
            <v-icon v-text="item.meta.icon" />
          </v-list-item-action>
          <v-list-item-content>
            <v-list-item-title v-text="item.meta.label || item.meta.title" />
          </v-list-item-content>
        </v-list-item>
        <v-list-item v-if="login" @click="logout">
          <v-list-item-action>
            <v-icon>exit_to_app</v-icon>
          </v-list-item-action>
          <v-list-item-content>
            <v-list-item-title>
              Logout
            </v-list-item-title>
          </v-list-item-content>
        </v-list-item>
      </v-list>
    </v-navigation-drawer>

    <v-app-bar app short>
      <v-app-bar-nav-icon @click.stop="drawer = !drawer" />
      <v-toolbar-title>
        {{ title }}
      </v-toolbar-title>
      <v-spacer />
      <v-toolbar-items>
        <v-btn text v-if="status && status.hostname">
          {{ status.hostname }}
        </v-btn>
      </v-toolbar-items>
    </v-app-bar>

    <v-content>
      <router-view
        v-if="bootComplete"
        :connect-dialog.sync="connectDialogVisible"
      />
      <v-container v-else class="fill-height" fluid>
        <v-row align="center" justify="center">
          <v-progress-circular
            indeterminate
            size="96"
            color="grey lighten-2"
            class="mx-auto"
          />
        </v-row>
      </v-container>
    </v-content>
    <div class="stacked-snackbar">
      <v-snackbar
        v-for="popup in popups"
        :key="popup.id"
        :color="popup.color"
        v-model="popup.state"
        :timeout="popup.timeout"
        class="mt-2"
      >
        {{ popup.message }}
        <v-btn icon @click="popup.state = false">
          <v-icon>close</v-icon>
        </v-btn>
      </v-snackbar>
    </div>
  </v-app>
</template>
<script lang="ts">
import Vue from 'vue'
import Component from 'vue-class-component';
import { Watch } from "vue-property-decorator";
import { mapState } from 'vuex';
import Api from './api';
import { Status } from './interfaces';

@Component({
  computed: mapState(['title', 'popups', 'bootComplete', 'login', 'status'])
})
export default class App extends Vue {
  connectDialogVisible: boolean = null

  get menuList(): any[] {
    ///@ts-ignore
    return (this.$router.options.routes as any[]).filter(
      v => !v.meta.hideMenu
    )
  }
  drawer: boolean = null

  // Typing helper
  status: Status

  logout() {
    Api.password = null
    this.$store.commit('login', false)
    this.$router.push('/')
  }
  mounted() {
    setTimeout(() => this.statusChecker(), 5000)
  }

  @Watch('status.mode')
  modeChanged(newMode, oldMode) {

    if ('undefined' == typeof oldMode) return;
    // if mode changed, load full config
    this.$store.dispatch('config')
  }


  statusChecker() {
    this.$store.dispatch("status").then(
      () => setTimeout(() => this.statusChecker(), 5000)
    )
  }
}
</script>