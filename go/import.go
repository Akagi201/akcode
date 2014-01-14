
// 1) 在import中，你可以使用相对路径，如 ./或 ../ 来引用你的package
// 2) 如果没有使用相对路径，那么，go会去找$GOPATH/src/目录。

import "./haoel" // import当前目录里haoel子目录里的所有的go文件
import "haoel"   // import 环境变量 $GOPATH/src/haoel子目录里的所有的go文件

