<template>
  <div>
    <el-table :data="this.detail.slice((currentPage-1)*pageSize,currentPage*pageSize)" style="height: 456px">
      <el-table-column property="address" label="详细地址" ></el-table-column>
      <el-table-column property="level" label="风险等级" width="100"
                       :filters="[{ text: '高', value: '高' }, { text: '中', value: '中' }]"
                       :filter-method="filterTag">
        <template slot-scope="scope">
          <el-tag
              :type="scope.row.level === '高' ? 'primary' : 'success'"
              disable-transitions>{{scope.row.level}}</el-tag>
        </template>
      </el-table-column>
    </el-table>
    <el-pagination
        :style="{'text-align' : 'center','margin-top' : '40px','margin-bottom' : '10px'}"
        :current-page=currentPage
        @current-change="handleCurrentChange"
        background
        :page-size=pageSize
        layout="total, prev, pager, next"
        :total=detail.length
    >
    </el-pagination>
  </div>
</template>

<script>
export default {
  name: "RiskAreaDetail",
  props: ['detail'],
  data(){
    return{
      isDialogVisible: false,
      currentPage : 1,
      pageSize : 5
    }
  },
  methods:{
   filterTag(value, row) {
     return row.level === value;
   },
  handleCurrentChange(cur){
     this.currentPage = cur;
    }
 }
}
</script>

<style scoped>
</style>