#!/bin/bash

echo hello; echo there

if [ -x "$filename"]; then # 注意: "if" 和 "then"需要分隔
    echo "File $filename exists."; cp $filename $filename.bak
else
    echo "File $filename not found."; touch $filename
fi; echo "File test complete."

case "$variable" in
abc) echo "\$variable = abc" ;;
xyz) echo "\$variable = xyz" ;;
esac

