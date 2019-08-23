package main

import (
	"context"
	"fmt"
	"github.com/kataras/iris"
	"go.mongodb.org/mongo-driver/mongo"
	"go.mongodb.org/mongo-driver/mongo/options"
	"time"
)

var client *mongo.Client

func init() {
	var err error
	client, err = mongo.NewClient(options.Client().ApplyURI("mongodb://localhost:27024"))
	if err != nil {
		fmt.Println(err)
		return
	}
	ctx, cancel := context.WithTimeout(context.Background(), 20 * time.Second )
	defer cancel()
	err = client.Connect(ctx)
	if err != nil {
		fmt.Println(err)
		return
	}
}

func main() {
	app := iris.New()

	app.Post("/weather", func (ctx iris.Context) {
		var reqJson map[string]interface{}
		err := ctx.ReadJSON(&reqJson)
		fmt.Printf("%s", &err)
		c := client.Database("test").Collection("weatherInfos")
		insertRes, err := c.InsertOne(context.Background(), reqJson)
		fmt.Printf("%s", &err)
		if err != nil {
			fmt.Println(err)
			return
		}
		fmt.Println("%s", insertRes)
	})

	app.Run(iris.Addr(":8080"))
}