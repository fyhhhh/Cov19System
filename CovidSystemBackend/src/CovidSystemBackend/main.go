package main

import (
   "fmt"
   "github.com/gin-gonic/gin"
   "gorm.io/driver/mysql"
   "gorm.io/gorm"
   "io"
   "os"
)
type Hospital struct {
   ID       int32
   Province string
   City     string
   District string
   Address  string
   Title    string
   Tel      string
   Lat      float64
   Lng      float64
}

type Detail struct {
   Id          int32
   UpdateTime  string
   Province    string
   City        string
   Confirm     int32
   ConfirmAdd  int32
   Heal        int32
   Dead        int32
}

type ProvinceTotal struct {
   UpdateTime       string
   Province         string
   TotalNow         int32
   TotalConfirmAdd  int32
   TotalHeal        int32
   TotalDead        int32
}

type CityTotal struct {
   UpdateTime        string
   Province          string
   City              string
   TotalNow          int32
   TotalConfirmAdd   int32
   TotalHeal         int32
   TotalDead         int32
}

type History struct{
   Ds          string
   Confirm     int32
   ConfirmAdd  int32
   Suspect     int32
   SuspectAdd  int32
   Heal        int32
   HealAdd     int32
   Dead        int32
   DeadAdd     int32
}

type RiskArea struct{
   ID          int32
   RiskLevel   string
   Address     string
}

type Report struct {
   IdReport    int32
   Province    string
   City        string
   County      string
   District    string
   Info        string
   Time        string
   Device      int32
}

type Abnormal struct {
   IdAbnormal  int32
   Province    string
   City        string
   County      string
   District    string
   Info        string
   Time        string
   Device      int32
}

type Province struct {
   Id    int32
   Code  string
   Name  string
}

type City struct {
   Id             int32
   Code           string
   Name           string
   ProvinceCode   string
}

type County struct {
   Id             int32
   Code           string
   Name           string
   CityCode       string
}

type Area struct{
   Province    string
   City        string
   County      string
}

func main(){
   //GORM连接数据库
   fileName := "./passwd.txt"
   file, err := os.Open(fileName)
   if err != nil{
      fmt.Println("Read file err, err =", err)
      return
   }
   defer file.Close()
   var chunk []byte
   buf := make([]byte, 1024)
   for{
      n, err := file.Read(buf)
      if err != nil && err != io.EOF{
         fmt.Println("read buf fail", err)
         return
      }
      //说明读取结束
      if n == 0 {
         break
      }
      //读取到最终的缓冲区中
      chunk = append(chunk, buf[:n]...)
   }
   dsn := "root:"+string(chunk)+"@(localhost:3306)/cov19?charset=utf8mb4&parseTime=True&loc=Local"
   db, err := gorm.Open(mysql.Open(dsn), &gorm.Config{})
   if err != nil{
      panic(err)
   }
   if db == nil {
      fmt.Println("DB open failed")
      return
   }else{
      fmt.Println("Database Cov19 open success")
   }

   //Gin处理Web请求
   r := gin.Default()

   r.GET("/details",func(c *gin.Context){
      provinceName := c.Query("province")
      cityName := c.Query("city")
      newest := c.Query("newest")

      if provinceName == "" { //没有省名字，报错
         var details []Detail
         result := db.Find(&details)
         if result.Error != nil {
            fmt.Println("Hospitals select failed")
         }
         c.JSON(200, details)
      } else{
         if cityName != "" { //有市的名字，详细查询
            if cityName == "all" { //all，查询所有
               var cityTotals []CityTotal
               var result *gorm.DB
               if newest == "true" {
                  subQuery := db.Model(&Detail{}).Select("update_time,province,city,if((confirm)-sum(heal)-sum(dead)<0,0,(confirm)-sum(heal)-sum(dead)) as total_now,sum(confirm_add) as total_confirm_add,sum(heal) as total_heal,sum(dead) as total_dead").Where("province=?", provinceName).Group("province,city,update_time")
                  result = db.Table("(?) as t",subQuery).Select("max(update_time)as update_time,province,city,total_now,total_confirm_add,total_heal,total_dead").Group("city").Find(&cityTotals)
               } else {
                  result = db.Model(&Detail{}).Select("update_time,province,city,if((confirm)-sum(heal)-sum(dead)<0,0,(confirm)-sum(heal)-sum(dead)) as total_now,sum(confirm_add) as total_confirm_add,sum(heal) as total_heal,sum(dead) as total_dead").Where("province=?", provinceName).Group("province,city,update_time").Find(&cityTotals)
               }
               if result.Error != nil {
                  fmt.Println("Details select failed")
               }
               c.JSON(200, cityTotals)
            } else{ //查询某一个市
               var cityTotals []CityTotal
               var result *gorm.DB
               if newest == "true" {
                  result = db.Model(&Detail{}).Select("update_time,province,city,if((confirm)-sum(heal)-sum(dead)<0,0,(confirm)-sum(heal)-sum(dead)) as total_now,sum(confirm_add) as total_confirm_add,sum(heal) as total_heal,sum(dead) as total_dead").Where("province=? and city=?", provinceName, cityName).Group("province,city,update_time").Order("update_time desc").First(&cityTotals)
               } else {
                  result = db.Model(&Detail{}).Select("update_time,province,city,if((confirm)-sum(heal)-sum(dead)<0,0,(confirm)-sum(heal)-sum(dead)) as total_now,sum(confirm_add) as total_confirm_add,sum(heal) as total_heal,sum(dead) as total_dead").Where("province=? and city=?", provinceName, cityName).Group("province,city,update_time").Find(&cityTotals)
               }
               if result.Error != nil {
                  fmt.Println("Details select failed")
               }
               c.JSON(200, cityTotals)
            }
         } else{ //没有市，查询省级信息
            if provinceName=="all" { //查询全部省级信息
               var provinceTotals []ProvinceTotal
               var result *gorm.DB
               if newest == "true" {
                  subQuery := db.Model(&Detail{}).Select("update_time,province,if(sum(confirm)-sum(heal)-sum(dead)<0,0,sum(confirm)-sum(heal)-sum(dead)) as total_now,sum(confirm_add) as total_confirm_add,sum(heal) as total_heal,sum(dead) as total_dead").Group("province,update_time")
                  result = db.Table("(?) as t",subQuery).Select("max(update_time)as update_time,province,total_now,total_confirm_add,total_heal,total_dead").Group("province").Find(&provinceTotals)
               } else {
                  result = db.Model(&Detail{}).Select("update_time,province,if(sum(confirm)-sum(heal)-sum(dead)<0,0,sum(confirm)-sum(heal)-sum(dead)) as total_now,sum(confirm_add) as total_confirm_add,sum(heal) as total_heal,sum(dead) as total_dead").Group("province,update_time").Find(&provinceTotals)
               }
               if result.Error != nil {
                  fmt.Println("Details select failed")
               }
               c.JSON(200, provinceTotals)
            } else{ //查询某一个省级信息
               var provinceTotals []ProvinceTotal
               var result *gorm.DB
               if newest == "true" {
                  result = db.Model(&Detail{}).Select("update_time,province,if(sum(confirm)-sum(heal)-sum(dead)<0,0,sum(confirm)-sum(heal)-sum(dead)) as total_now,sum(confirm_add) as total_confirm_add,sum(heal) as total_heal,sum(dead) as total_dead").Where("province=?", provinceName).Group("province,update_time").Order("update_time desc").First(&provinceTotals)
               } else {
                  result = db.Model(&Detail{}).Select("update_time,province,if(sum(confirm)-sum(heal)-sum(dead)<0,0,sum(confirm)-sum(heal)-sum(dead)) as total_now,sum(confirm_add) as total_confirm_add,sum(heal) as total_heal,sum(dead) as total_dead").Where("province=?", provinceName).Group("province,update_time").Find(&provinceTotals)
               }
               if result.Error != nil {
                  fmt.Println("Details select failed")
               }
               c.JSON(200, provinceTotals)
            }
         }
      }

   })

   r.GET("/histories", func(c *gin.Context) {
      var histories []History
      result := db.Find(&histories)
      if result.Error != nil {
         fmt.Println("Histories select failed")
      }
      c.JSON(200, histories)

   })

   r.GET("/risk_areas", func(c *gin.Context) {
      var riskAreas []RiskArea
      result := db.Find(&riskAreas)
      if result.Error != nil{
         fmt.Println("RiskAreas select failed")
      }
      c.JSON(200, riskAreas)
   })

   r.GET("/hospitals", func(c *gin.Context) {
      provinceName := c.Query("province")
      cityName := c.Query("city")
	  districtName := c.Query("district")
      if provinceName == "" || cityName == "" || districtName == ""{
         c.JSON(200, gin.H{
            "error":"select province and city needed",
         })
      } else {
         var hospitals []Hospital
         result := db.Where("Province=? and City=? and District=?",provinceName,cityName,districtName).Find(&hospitals)
         if result.Error != nil {
            fmt.Println("Hospitals select failed")
         }
         c.JSON(200, hospitals)
      }
   })

   r.GET("/reports", func(c *gin.Context) {
      provinceName := c.Query("province")
      cityName := c.Query("city")
      countyName := c.Query("county")
      if provinceName == "" || cityName == "" || countyName == ""{
         c.JSON(200, gin.H{
            "error":"select province and city needed",
         })
      } else {
         var reports []Report
         result := db.Where("Province=? and City=? and County=?",provinceName,cityName,countyName).Find(&reports)
         if result.Error != nil {
            fmt.Println("Reports select failed")
         }
         c.JSON(200, reports)
      }
   })
   
   r.POST("/reports", func(c *gin.Context) {
      json := make(map[string]interface{}) //注意该结构接受的内容
      c.BindJSON(&json)
      c.String(200,"received")
      var abnormal Abnormal
      db.Model(&Abnormal{}).Where("province=? and city=? and county=? and district=? and info=? and time=? and device=?",json["Province"],json["City"],json["County"],json["District"],json["Info"],json["Time"],json["Device"]).Delete(&abnormal)
      db.Model(&Report{}).Create(json)
   })

   r.POST("/reports_delete", func(c *gin.Context) {
      json := make(map[string]interface{}) //注意该结构接受的内容
      c.BindJSON(&json)
      c.String(200,"received")
      var report Report
      db.Where("province=? and city=? and county=? and district=? and info=? and time=? and device=?",json["Province"],json["City"],json["County"],json["District"],json["Info"],json["Time"],json["Device"]).Delete(&report)
   })

   r.GET("/abnormal", func(c *gin.Context) {
      provinceName := c.Query("province")
      cityName := c.Query("city")
      countyName := c.Query("county")
      if provinceName == "" || cityName == "" || countyName == ""{
         var abnormals []Abnormal
         result := db.Find(&abnormals)
         if result.Error != nil {
            fmt.Println("Reports select failed")
         }
         c.JSON(200, abnormals)
      } else {
         var abnormals []Abnormal
         result := db.Model(&Abnormal{}).Where("Province=? and City=? and County=?",provinceName,cityName,countyName).Find(&abnormals)
         if result.Error != nil {
            fmt.Println("Reports select failed")
         }
         c.JSON(200, abnormals)
      }
   })

   r.POST("/abnormal", func(c *gin.Context) {
      json := make(map[string]interface{}) //注意该结构接受的内容
      c.BindJSON(&json)
      c.String(200,"received")
      db.Model(&Abnormal{}).Create(json)
   })

   r.POST("/abnormal_delete", func(c *gin.Context) {
      json := make(map[string]interface{}) //注意该结构接受的内容
      c.BindJSON(&json)
      c.String(200,"received")
      var abnormal Abnormal
      db.Where("province=? and city=? and county=? and district=? and info=? and time=? and device=?",json["Province"],json["City"],json["County"],json["District"],json["Info"],json["Time"],json["Device"]).Delete(&abnormal)
   })

   r.GET("/area", func(c *gin.Context) {
      provinceName := c.Query("province")
      cityName := c.Query("city")
      if provinceName == "" && cityName == ""{
         var areas []Area
         subQuery := db.Model(&County{}).Select("cities.province_code,cities.name as city,counties.name as county ").Joins("left join cities on counties.city_code=cities.code")
         result := db.Model(&Province{}).Select("provinces.name as province,t.city,t.county ").Joins("right join (?) as t on t.province_code=provinces.code",subQuery).Find(&areas)
         if result.Error != nil {
            fmt.Println("Reports select failed")
         }
         c.JSON(200, areas)
         return
      }
      if provinceName == "all"{
         var provinces []Province
         var provinceNames []string
         result := db.Model(&Province{}).Find(&provinces)
         if result.Error != nil {
            fmt.Println("Reports select failed")
         }
         for i := range provinces {
            provinceNames = append(provinceNames,provinces[i].Name)
         }
         c.JSON(200, provinceNames)
         return
      }
      if provinceName != ""{
         var cities []City
         var cityNames []string
         subQuery := db.Model(&Province{}).Select("Code").Where("name=?",provinceName)
         result := db.Model(&City{}).Where("province_code in (?)", subQuery).Find(&cities)
         if result.Error != nil {
            fmt.Println("Reports select failed")
         }
         for i := range cities{
            cityNames = append(cityNames,cities[i].Name)
         }
         c.JSON(200,cityNames)
         return
      }
      if cityName != ""{
         var counties []County
         var countyNames []string
         subQuery := db.Model(&City{}).Select("Code").Where("name=?",cityName)
         result := db.Model(&County{}).Where("city_code in (?)", subQuery).Find(&counties)
         if result.Error != nil {
            fmt.Println("Reports select failed")
         }
         for i := range counties{
            countyNames = append(countyNames,counties[i].Name)
         }
         c.JSON(200,countyNames)
         return
      }
   })
   
   r.Run()
}