<template>
  <div class="box-card" style="padding-left: 0px;">
    <DetailCard :icon="'el-icon-info'" :name="selectSet[0]" :value="details.Position"></DetailCard>
    <DetailCard :icon="'el-icon-user-solid'" :name="selectSet[1]" :value="details.TotalNow"></DetailCard>
    <DetailCard :icon="'el-icon-s-help'" :name="selectSet[2]" :value="details.TotalHeal"></DetailCard>
    <DetailCard :icon="'el-icon-user'" :name="selectSet[3]" :value="details.TotalDead"></DetailCard>
  </div>
</template>

<script>
import DetailCard from "./DetailCard";
export default {
  name: "ShowDetials",
  components: {DetailCard},
  props: ['details'],
  data(){
    return {
      selectSet : ['当前地区','现存确诊','累计治愈','累计死亡','累计确诊']
    }
  }
}
</script>

<style scoped>


.text {
  font-size: 14px;
}

.item {
  display: inline-block;
}

.clearfix:before,
.clearfix:after {
  display: table;
  content: "";
}
.clearfix:after {
  clear: both
}

.box-card {
  margin: 56px 0px;
}
</style>