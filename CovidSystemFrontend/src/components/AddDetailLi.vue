<template>
  <el-card class="box-card" :style = 'style'>
    <div  class="text item">
      {{listName}} : {{value}}
    </div>
  </el-card>
</template>

<script>
export default {
  name: "AddDetailLi",
  props: ['listName','value'],
  data(){
    return{
      style:{
        'border-radius': '20px',
        'margin-bottom': '10px',
        'width': '100%'
      }
    }
  }
}
</script>

<style scoped>

</style>