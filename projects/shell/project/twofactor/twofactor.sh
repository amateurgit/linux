#/bin/bash

export TOPDIR=$(pwd)

# 定义脚本选项
ARGS=$(getopt -o h --long add_user_test,help,workflow,db_two_factor,admin-login,config-mailserver,send_captcha,test-login-with-captcha: -n "$0" -- "$@")
if [ $? != 0 ]; then
    exit 1
fi

# 重新初始化位置参数
eval set -- "${ARGS}"

while true; do
    case "$1" in
        --add_user_test)
	    webapp/scripts/add_user.sh
            shift
            ;;
        --test-login-with-captcha)
	    webapp/scripts/test_login.sh $2
            shift 2
            ;;
        --config-mailserver)
	    webapp/scripts/config_mailserver.sh
            shift
            ;;
        --db_two_factor)
	    database/create_two_factor.sh
            shift
            ;;
        -h|--help)
	    help/usage.sh
            shift
            ;;
        --admin-login)
	    webapp/scripts/admin_login.sh
            shift
            ;;
        --send_captcha)
	    webapp/scripts/send_captcha.sh
            shift
            ;;
        --workflow)
	    help/workflow.sh
            shift
            ;;
        --)                                       # 选项处理完毕, 退出循环
            shift
            break
            ;;
        *)
            echo "内部错误"
	    break
            ;;    
    esac
done
