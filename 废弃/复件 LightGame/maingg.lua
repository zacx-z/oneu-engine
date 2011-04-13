require "graphics"

commander.display(true)

g = group.new(0)
group.open(g)
p = sprite.new("letters.png", 2)
group.close()
g2 = group.new(2)
group.open(g2)
p2 = sprite.new("letters.png", 3)
group.close()

function main()end
function destroy() 
	sprite.remove(p)
	sprite.remove(p2)
	group.remove(g)
	group.remove(g2)
end
