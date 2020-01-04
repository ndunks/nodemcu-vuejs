
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
      <v-toolbar-title v-text="title"></v-toolbar-title>
    </v-app-bar>

    <v-content>
      <router-view v-if="bootComplete" />
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
    <v-footer color="indigo" app>
      <span class="white--text">&copy; 2020</span>
    </v-footer>
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
import { mapState } from 'vuex';
import Api from './api';

@Component({
  computed: {
    ...mapState(['title', 'popups', 'bootComplete', 'login'])
  }
})
export default class App extends Vue {
  get menuList(): any[] {
    ///@ts-ignore
    return (this.$router.options.routes as any[]).filter(
      v => !v.meta.hideMenu
    )
  }
  drawer: boolean = null
  logout() {
    Api.password = null
    this.$store.commit('login', false)
    this.$router.push('/')
  }
}
</script>