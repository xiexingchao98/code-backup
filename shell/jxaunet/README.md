# jxaunet
## jxau校园网登陆脚本

## 准备
```shell
cp jxaunet ~/
cd ~/jxaunet
mv jxaunet.sh jxaunet
sudo sh -c  "echo -e 'export PATH=\${PATH}:~/jxaunet' >> /etc/profile.d/custom.sh"
sudo sh -c  "echo 'export JXAUNET_HOME=~/jxaunet' >> /etc/profile.d/custom.sh"
source /etc/profile
```
## 编辑配置文件
```shell
vim ～/jxaunet/jxaunet.config

# 用户ID
userid=
# 密码
password=
```

## 使用
```shell
jxaunet [login|logout]
```