package main

import (
	"fmt"
	"net/http"
	"sync"
	"time"
)

const (
	// 测试次数
	testCount = 15
	// 要访问的资源路径
	resourcePath = "/assets/logo__on-dark.svg"
	protocolHttp = "http://"
)

// 测试结果
var testResult = make(map[string][]int)

var wg sync.WaitGroup
var mutex sync.Mutex

var httpClient *http.Client

// 服务器列表
var server = map[string]string {
	"Frankfurt, DE": "fra-de-ping.vultr.com",
	"Paris, France": "par-fr-ping.vultr.com",
	"Amsterdam, NL": "ams-nl-ping.vultr.com",
	"London, UK": "lon-gb-ping.vultr.com",
	"New York (NJ) ": "nj-us-ping.vultr.com",
	"Singapore": "sgp-ping.vultr.com",
	"Toronto, Canada": "tor-ca-ping.vultr.com",
	"Chicago, Illinois": "il-us-ping.vultr.com",
	"Atlanta, Georgia ": "ga-us-ping.vultr.com",
	"Miami, Florida ": "fl-us-ping.vultr.com",
	"Tokyo, Japan ": "hnd-jp-ping.vultr.com",
	"Dallas, Texas": "tx-us-ping.vultr.com",
	"Seattle, Washington ": "wa-us-ping.vultr.com",
	"Silicon Valley, California": "sjo-ca-us-ping.vultr.com",
	"Los Angeles, California": "lax-ca-us-ping.vultr.com",
	"Sydney, Australia": "syd-au-ping.vultr.com",
}

func main() {
	httpClient = &http.Client{
		Timeout: 2 * time.Second,
	}

	showTopBar()

	wg.Add(len(server) * testCount)

	for location, domain := range server {
		go testServer(location, domain)
	}

	wg.Wait()
}

func testServer(location, domain string) {
	for i := 0; i < testCount; i++ {
		before := time.Now()

		if _, err := httpClient.Get(protocolHttp + domain + resourcePath); err != nil {
			mutex.Lock()
			testResult[location] = append(testResult[location], -1)
		} else {
			mutex.Lock()
			testResult[location] = append(testResult[location], (int)(time.Since(before) / 1e6))
		}

		if len(testResult[location]) == testCount {
			showLocationStatistic(location)
		}

		mutex.Unlock()
		wg.Done()
	}
}

func showTopBar() {
	fmt.Printf("\n%-32s | %-10s | %-10s | %-10s | %-10s\n", "Location", "Avg(ms)", "Max(ms)", "Min(ms)", "Loss(%)")
	printDivider()
}

func showLocationStatistic(location string) {
	data := testResult[location]
	max := data[0]
	min := 99999
	sum := 0
	invalid := 0

	for _, delay := range data {
		if delay == -1 {
			invalid++
			continue
		}
		sum += delay
		if delay > max {
			max = delay
		} else if delay < min {
			min = delay
		}
	}
	tmp := testCount - invalid
	avg := 0
	if tmp != 0 {
		avg = sum / tmp
	}
	fmt.Printf("%-32s | %-10d | %-10d | %-10d | %7.2f\n", location, avg, max, min, (float64(invalid) / testCount) * 100 )
}

func printDivider() {
	for i := 0; i <84; i++ {
		fmt.Printf("-")
	}
	fmt.Printf("\n")
}