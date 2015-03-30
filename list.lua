function addtail (l, e)
  if l.next == nil then
    l.next = e
    return
  end
  addtail(l.next, e)
end

function plist (l)
  if not l then return end
  print(l, l.val, l.next)
  plist(l.next)
end

head = {val = 0}
e1 = {val = 1}
e2 = {val = 2}

print "element\t\t\tvalue\tnext"
plist(head)
print ""
addtail(head, e2)
plist(head)
addtail(head, e1)
print ""
plist(head)

