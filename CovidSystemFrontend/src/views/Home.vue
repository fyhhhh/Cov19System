<template>
  <el-container style="height: 800px; ">
    <el-aside width="200px" style="background-color: #303133">
      <el-menu >
        <div class="menu-title">
            <span class="text" >CovidSystemPanel</span>
            <span class="text" style="font-size: 16px;font-weight:normal">V1.0.0</span>
        </div>
        <router-link to="/map" tag="el-menu-item" ><i class="el-icon-s-grid"/>疫情地图</router-link>
        <router-link to="/riskArea" tag="el-menu-item" ><i class="el-icon-s-flag"/>风险地区</router-link>
        <router-link to="/hospital" tag="el-menu-item" ><i class="el-icon-first-aid-kit"/>核酸检测点</router-link>
        <router-link to="/route" tag="el-menu-item" ><i class="el-icon-user-solid"/>个人流调</router-link>
      </el-menu>
    </el-aside>

    <el-container>
      <el-main style="padding: 5px">
        <router-view/>
      </el-main>
    </el-container>
  </el-container>
</template>

<script>
export default {
  name: "Home"
}
</script>

<style scoped>
.el-menu-item{
  background-color: #303133;
  color: #eeeeee;
  width: 200px
}

.el-menu-item:hover{
  background: #606266;
}

.el-header {
  background-color: #303133;
  color: #333;
  line-height: 60px;
}

.el-aside {
  color: gray;
}
.menu-title{
  background-color: #303133;
  height: 80px;
  width: 200px
}
.text{
  color: #eeeeee;
  background-color: #303133;
  display: block;
  font-size: 18px;
  top:10px;
  text-align: left;
  font-weight: bold;
  padding-top: 10px;
}
</style>