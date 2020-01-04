<template>
  <v-container class="fill-height" fluid>
    <v-col xs="8" sm="6" md="4" class="mx-auto">
      <v-card>
        <v-card-title>Login</v-card-title>
        <v-card-text>
          <v-form @submit.prevent="submit">
            <v-text-field hide-details v-model="password" label="Password" />
          </v-form>
        </v-card-text>
        <v-card-actions>
          <v-spacer />
          <v-btn @click.stop="submit" text color="success">Login</v-btn>
        </v-card-actions>
      </v-card>
    </v-col>
  </v-container>
</template>
<script lang="ts">
import Vue from 'vue'
import Component from 'vue-class-component';
import { Popup } from '../interfaces';
import Api from '../api';

@Component
export default class Login extends Vue {
  password: string = null
  submit() {
    if (!this.password) {
      return this.$store.commit('popup', 'Please input password')
    }
    Api.password = this.password
    console.log(Api);
    Api.get('ping').then(
      resp => {
        this.$store.commit('login', true)
        this.$router.replace(this.$route.query.next as string || '/dash')
      }
    )

  }
}
</script>

