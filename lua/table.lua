
-- table.concat, table.insert, table.maxn, table.remove, table.sort

a = {2}
table.insert(a, 3);
table.insert(a, 4);
table.sort(a, function(v1, v2) return v1 > v2 end)
for i,v in ipairs(a) do print(i,v) end

