#!/bin/bash
url_api="10.255.240.8/eportal/InterFace.do?method="
url_logout_redirect="10.255.240.8/eportal/gologout.jsp"

checkEnv() {
    if [[ ! $JXAUNET_HOME || -z $JXAUNET_HOME ]]
    then
        echo "无法找到环境变量JXAUNET_HOME"
        exit 1
    fi
}

doLogin() {
    checkEnv
    redirect_script=`curl -s "123.123.123.123"`
    tmp=${redirect_script#*\?}
    query_string=${tmp%\'*}
    while read line
    do
        eval "${line}"
    done < "${JXAUNET_HOME}/jxaunet.config"
    curl "${url_api}login" --data-urlencode "userId=${userid}" --data-urlencode "password=${password}" --data-urlencode "queryString=${query_string}"
}

doLogout() {
    location=`curl -I "${url_logout_redirect}" | grep "Location"`
    user_index=${location##*?}
    curl "${url_api}logout" --data-urlencode "${user_index}"
}

case $1 in
    "logout" )
        doLogout
    ;;
    "login" )
        doLogin
    ;;
    * )
        echo "错误的选项"
    ;;
esac


