require "table_ext"
userlib = {
	-- returns user string or false
	get_user = function()
		local file = io.open("config.dat", "r")
		if not file then return false end
		local curuser = file:read('*l')
		file:close()
		return curuser
	end,
	set_user = function(user)
		local file = io.open("config.dat", "w")
		file:write(user)
		file:close()
	end,
	load_dict = function()
		if userlib.dict then return end

		local file = io.open("words.txt", "r")
		if not file then error('cannot find "words.txt"') end

		userlib.dict = {}
		local newline = file:read('*l')
		while newline do
			local word = string.gmatch(newline, '%w+')()
			local chinese = string.gsub(newline, '(%s*)(%w+)(%s+)(.+)', '%4')
			table.insert(userlib.dict, {word = word, chinese = chinese})
			newline = file:read('*l')
		end
		file:close()
	end,
	open = function(user)
		local file = io.open(user..'.dat', 'r')
		if not file then
			local t = {}
			t.score = 0
			t.list = {}
			for i = 1, #userlib.dict do
				table.insert(t.list, {fam = 0, times = 0, last_time = 0, id = i})
			end
			return t
		end
		local usertable = {}
		usertable.score = file:read('*n') -- how many words you have answered
		usertable.list = {}
		local n = file:read('*n')
		local m = file:read('*n')
		local d = file:read('*n')
		local i = 0
		while n do
			i = i + 1
			table.insert(usertable.list, {fam = n, times = m, last_time = d, id = i})
			n = file:read('*n')
			m = file:read('*n')
			d = file:read('*n')
		end
		file:close()
		return usertable
	end,
	sort = function(list)
		local now = os.time()
		local prior = function(v)
			-- caculates the priorities
			-- 你可以修改这个函数使其符合你的需要
			-- v.fam 熟悉度 答对+1 单词落到底会使熟悉度慢慢下降
			-- v.times 答中次数
			-- v.last_time 上次答中的时间
			-- 局部变量
			-- now 现在的时间
			if v.times == 0 then return 1 end
			if v.fam < 15 then return (now - v.last_time) / v.times / v.fam + 1
			else return 0 end -- when familiarity reaches 10 then do not recite it
		end
		return table.sort(list,
			function(a, b)
				return prior(a) > prior(b)
			end)
	end,
	save = function(user, usertable)
		local t = table.sort(usertable.list, function(a, b) return a.id < b.id end)
		local file = io.open(user..'.dat', 'w')
		file:write(usertable.score, '\n')
		for _, v in ipairs(t) do
			file:write(v.fam, ' ', v.times, ' ', v.last_time, '\n')
		end
		file:close()
	end
}

