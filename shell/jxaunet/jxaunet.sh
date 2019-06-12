#!/bin/bash

url_api="10.255.240.8/eportal/InterFace.do?method="
url_logout_redirect="10.255.240.8/eportal/gologout.jsp"

doLogin() {
    redirect_script=`curl -s "123.123.123.123"`
    tmp=${redirect_script#*\?}
    query_string=${tmp%\'*}
    while read line
    do
        eval "${line}"
    done < jxaunet.config
    curl "${url_api}login" --data-urlencode "userId=${userid}" --data-urlencode "password=${password}" --data-urlencode "queryString=${query_string}"
}

doLogout() {
    location=`curl "${url_logout_redirect}" | grep "Location"`
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
        echo "option wrong!"
    ;;
esac


