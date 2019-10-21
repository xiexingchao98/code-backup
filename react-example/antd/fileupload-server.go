package main

import (
	"fmt"
	"github.com/kataras/iris"
	"io/ioutil"
	"os"
)

func main() {
	app := iris.New()

	/*
	 * 客户端会提前向服务端发送 OPTIONS 请求，以确定请求头部合法参数
	 * 调试阶段，因为后端与前端所属使用域名不同（端口），所以需要允许跨域请求
	 */
	app.Options("/upload", func(ctx iris.Context) {
		ctx.Header("Access-Control-Allow-Origin", "*")
		ctx.Header("Access-Control-Allow-Headers", "x-requested-with")
	})

	/*
	 * 处理文件上传请求
	 */
	app.Post("/upload", func(ctx iris.Context){
		// 业务逻辑代码
		// 此处只是简单读取文件并保存至服务端
		file, header, err := ctx.FormFile("file")
		if err != nil {
			fmt.Println(err)
			panic(err)
		}

		// 确保 upload 目录已经存在
		output, err := os.Create("./upload/" + header.Filename)
		bts, err := ioutil.ReadAll(file)
		if _, err = output.Write(bts); err != nil {
			fmt.Println(err)
			panic(err)
		}

		_ = output.Close()
		_ = file.Close()

		// 允许跨域请求（必须设置）
		ctx.Header("Access-Control-Allow-Origin", "*")
		ctx.Header("Access-Control-Allow-Headers", "x-requested-with")
	})

	app.Run(iris.Addr(":9090"))
}
