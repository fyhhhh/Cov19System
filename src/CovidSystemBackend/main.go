package main

import (
   "fmt"
   "github.com/gin-gonic/gin"
   "gorm.io/driver/mysql"
   "gorm.io/gorm"
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
   Id       int32
   Province string
   City     string
   District string
   Info     string
}

func main(){
   //GORM连接数据库
   dsn := "root:156324@(localhost:3306)/cov19?charset=utf8mb4&parseTime=True&loc=Local"
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
      if provinceName == "" {
         var details []Detail
         result := db.Find(&details)
         if result.Error != nil {
            fmt.Println("Hospitals select failed")
         }
         c.JSON(200, details)
      } else{
         if cityName != "" {
            var cityTotals []CityTotal
            var result *gorm.DB
            if newest == "true" {
               result = db.Model(&Detail{}).Select("update_time,province,city,sum(confirm)-sum(heal)-sum(dead) as total_now,sum(confirm_add) as total_confirm_add,sum(heal) as total_heal,sum(dead) as total_dead").Where("province=? and city=?", provinceName, cityName).Group("province,city,update_time").Order("update_time desc").First(&cityTotals)
              } else {
               result = db.Model(&Detail{}).Select("update_time,province,city,sum(confirm)-sum(heal)-sum(dead) as total_now,sum(confirm_add) as total_confirm_add,sum(heal) as total_heal,sum(dead) as total_dead").Where("province=? and city=?", provinceName, cityName).Group("province,city,update_time").Find(&cityTotals)
            }
            if result.Error != nil {
               fmt.Println("Details select failed")
            }
            c.JSON(200, cityTotals)
         } else{
            var provinceTotals []ProvinceTotal
            var result *gorm.DB
            if newest == "true"{
               result = db.Model(&Detail{}).Select("update_time,province,sum(confirm)-sum(heal)-sum(dead) as total_now,sum(confirm_add) as total_confirm_add,sum(heal) as total_heal,sum(dead) as total_dead").Where("province=?",provinceName).Group("province,update_time").Order("update_time desc").First(&provinceTotals)
            } else {
               result = db.Model(&Detail{}).Select("update_time,province,sum(confirm)-sum(heal)-sum(dead) as total_now,sum(confirm_add) as total_confirm_add,sum(heal) as total_heal,sum(dead) as total_dead").Where("province=?", provinceName).Group("province,update_time").Find(&provinceTotals)
            }
            if result.Error != nil {
               fmt.Println("Details select failed")
            }
            c.JSON(200, provinceTotals)
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
      if provinceName == "" || cityName == "" {
         c.JSON(200, gin.H{
            "error":"select province and city needed",
         })
      } else {
         var hospitals []Hospital
         result := db.Where("Province=? and City=?",provinceName,cityName).Find(&hospitals)
         if result.Error != nil {
            fmt.Println("Hospitals select failed")
         }
         c.JSON(200, hospitals)
      }
   })

   r.GET("/reports", func(c *gin.Context) {
      provinceName := c.Query("province")
      cityName := c.Query("city")
      if provinceName == "" || cityName == "" {
         c.JSON(200, gin.H{
            "error":"select province and city needed",
         })
      } else {
         var reports []Report
         result := db.Where("Province=? and City=?",provinceName,cityName).Find(&reports)
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
      db.Model(&Report{}).Create(json)
   })

   r.POST("/reports_delete", func(c *gin.Context) {
      json := make(map[string]interface{}) //注意该结构接受的内容
      c.BindJSON(&json)
      c.String(200,"received")

      var report Report
      db.Where("province=? and city=? and district=? and info=?",json["Province"],json["City"],json["District"],json["Info"]).Delete(&report)
   })

   r.Run()
}