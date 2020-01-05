<template>
  <v-container fluid>
    <v-row>
      <v-col>
        <widget-wifi />
      </v-col>
    </v-row>
  </v-container>
</template>
<script lang="ts">
import Vue from 'vue'
import Component from 'vue-class-component';
import WidgetWifi from "@/widget/WidgetWifi.vue";

@Component({
  components: { WidgetWifi }
})
export default class Dashboard extends Vue {

}
</script>
