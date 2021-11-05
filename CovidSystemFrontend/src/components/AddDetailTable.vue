<template>
  <div class="table">
    <add-detail-li v-for="(item,key) in dataTable" :key="key" :value="item.value" :list-name="item.listName"></add-detail-li>
  </div>
</template>

<script>
import AddDetailLi from "./AddDetailLi";
export default {
  name: "AddDetailTable",
  props:['history'],
  components: {AddDetailLi},
  data(){
    return{
      dataTable:[
      ]
    }
  },
  mounted() {
      this.dataTable = [
        {listName: '昨日确诊新增', value: this.history.confirmAdd},
        {listName: '昨日治愈新增', value: this.history.healAdd},
        {listName: '昨日死亡新增', value: this.history.deadAdd},
      ]
  }
}
</script>

<style scoped>
</style>