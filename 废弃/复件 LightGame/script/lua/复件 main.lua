require "graphics_ext"
require "input"
require "internal_input_word"
require "internal_user_manage"
require "windows"
--author: ladace
--game parameters "letters.png", 0
--你可以修改参数 改变难度
flick_interval = 500 --单词每个字母的闪动周期
word_interval = 300 --单词出现周期
word_velocity = 0.5 --单词下落速度

--commander.display(true)

math.randomseed(os.time()) --reset random seed

function enter_scene(the_new_scene)
	scene:escape()
	scene = the_new_scene
	scene:enter()
end
--word sprite
cWordSprite = Object:derive({})
--new a cWordSprite
--contents is a string
function cWordSprite:__init(arg)
	local contents = arg.contents
	local x = arg.x or 0
	local y = arg.y or 0
	local width = arg.width -- may be nil
	local height = arg.height -- may be nil

	self.contents = contents
	self.sp = {}
	self.g = renderer.new_group(arg.z or 0)
	renderer.open_group(self.g)
	for i = 1, #contents do
		-- assert that all the letters is in lower case
		local ord = string.byte(contents, i) - string.byte("a") + 1
		if ord <= 26 and ord > 0 then
			local sp = cSprite:new{file = "letters.png"}
			--sp:set_scalex(1/26)
			sp:set_srcrect((ord - 1) / 26.0, 0, ord / 26.0, 1)
			sp:set_height(64)
			if(width) then
				sp:set_width(width)
			end
			if(height) then
				sp:set_height(height)
			end
			local width = width or sp:get_width()

			cWordSprite.s_width = width

			sp:set_x(x + width * i - width)
			sp:set_y(y)
			self.sp[i] = sp
		end
	end
	renderer.close_group()
end
cWordSprite.destroy = function(self)
	for i,v in pairs(self.sp) do
		v:destroy()
	end
	renderer.remove_group(self.g)
end
--title_scene
--output the title
title_scene = {
	enter = function(self)
		-- initialize title scene
		self.title = cWordSprite:new{contents = "game word", width = 60, height = 120, x = 10, y = 20}
		self.font = cFont:new{height = 30, width = 20}
		self.menu = {}
		local text = cText:new{contents = "start", font = self.font}
		text:set_rect{l =100, r = 500, t = 200, b = 500}
		text:set_color(50, 70, 120)
		self.menu[1] = text

		text = cText:new{contents = "choose user", font = self.font}
		text:set_rect{l = 130, r = 500, t = 300, b = 500}
		text:set_color(0, 24, 50)
		self.menu[2] = text

		text = cText:new{contents = "quit", font = self.font}
		text:set_rect{l = 160, r = 500, t = 400, b = 500}
		text:set_color(0, 24, 50)
		self.menu[3] = text

		-- global user
		user = userlib.get_user()
		text = cText:new{contents = "user:"..user or "no user", font = self.font}
		text.set_username = function(self, name) self:set_contents("user:"..name) end
		text:set_color(128, 128, 255)
		text:set_rect{l = 500, t = 130, r = 800, b = 200}
		self.user_ui = text
	end,
	closure_update = function(i)
		-- declare some internal variables here
		local index = i or 1
		-- then body of function
		return function(self)
			--self.title:set_x(self.title:get_x() + 1)
			if(input.key_release(input.IK_DOWN)) then
				if index < 3 then
					self.menu[index]:set_color(0, 24, 50)
					index = index + 1
					self.menu[index]:set_color(50, 70, 120)
				end
			end
			if(input.key_release(input.IK_UP)) then
				if index > 1 then
					self.menu[index]:set_color(0, 24, 50)
					index = index - 1
					self.menu[index]:set_color(50, 70, 120)
				end
			end
			if(input.key_release(input.IK_RETURN)) then
				-- start game
				if index == 1 then
					if not user then
						self.newuser = ""
						self.input_user_ui = cText:new{font = self.font, contents = ""}
						self.input_user_ui:set_rect{l = 240, t = 250, b = 300, r = 400}
						self.please_input_your_name = cText:new{font = self.font, contents = "please input your name"}
						self.please_input_your_name:set_rect{l = 150, t = 220, b = 250, r = 650}
						local quit_input = function()
							self.newuser = nil;
							self.input_user_ui:destroy();
							self.input_user_ui = 0;
							self.please_input_your_name:destroy()
							self.please_input_your_name = nil
							self.update = self.game_update
						end
						self.update = function()
							if input.key_release(input.IK_ESCAPE) then
								quit_input()
							end
							if self.newuser ~= "" and input.key_release(input.IK_RETURN) then
								user = self.newuser
								quit_input()
								userlib.set_user(user)
								enter_scene(game_scene)
							end

							local str = get_input_letter()

							if str and #self.newuser < 7 then self.newuser = self.newuser..str; self.input_user_ui:set_contents(self.newuser) end
						end
						return
					end
					enter_scene(game_scene)
				end
				-- choose user
				if index == 2 then
					self.newuser = ""
					self.input_user_ui = cText:new{font = self.font, contents = ""}
					self.input_user_ui:set_rect{l = 240, t = 350, b = 400, r = 400}
					self.please_input_your_name = cText:new{font = self.font, contents = "please input your name"}
					self.please_input_your_name:set_rect{l = 150, t = 320, b = 350, r = 650}
					local quit_input = function()
						self.newuser = nil;
						self.input_user_ui:destroy();
						self.input_user_ui = 0;
						self.please_input_your_name:destroy()
						self.please_input_your_name = nil
						self.update = self.game_update
					end
					self.update = function()
						if input.key_release(input.IK_ESCAPE) then
							quit_input()
						end
						if self.newuser ~= "" and input.key_release(input.IK_RETURN) then
							user = self.newuser
							userlib.set_user(user)
							self.user_ui:set_username(user)
							quit_input()
						end

						local str = get_input_letter()
						if str and #self.newuser < 7 then self.newuser = self.newuser..str; self.input_user_ui:set_contents(self.newuser) end
					end
				end
				-- quit
				if index == 3 then
					quit()
				end
			end
		end
	end,
	escape = function(self)
		-- destroy title scene
		self.title:destroy()
		for i,v in ipairs(self.menu) do
			v:destroy()
		end
		self.user_ui:destroy()
		self.font:destroy()
	end
}
title_scene.game_update = title_scene.closure_update()
title_scene.update = title_scene.game_update


--cWord
cWord = cWordSprite:derive({
})
function cWord:new(arg)
	if not cWord_font then cWord_font = cFont:new{width = 7, height = 14, facename = "黑体"} end

	local ins = cWordSprite:new(arg)
	ins.frame = {}
	ins.chinese = cText:new{font = cWord_font, contents = arg.chinese} --arg.chinese
	for i, v in pairs(ins.sp) do
		v:set_alpha(0)
	end
	ins.chinese:set_color(200, 245, 50)
	ins.chinese:set_rect{t = arg.y or 0, b = 600, l = 0, r = 800}

	ins.update = function(self)
		for i,v in pairs(self.frame) do
			self.frame[i] = (v + 1) % word_interval
			if v < 50 then
				self.sp[i]:set_alpha(v * 255 / 50)
			elseif v <= 100 then
				self.sp[i]:set_alpha((100 - v) * 255 / 50)
			end
		end
	end
	ins.set_y = function(self, y)
		self.chinese:set_rect{t = y, b = 600, l = 0, r = 800}
		for _, v in pairs(self.sp) do
			v:set_y(y)
		end
	end
	ins.set_x = function(self, x)
		for i, v in pairs(self.sp) do
			v:set_x(x + (i - 1) * self.s_width)
		end
	end
	ins.get_y = function(self)
		local sp = self.sp[1]
		return (sp and sp:get_y()) or 0
	end
	ins.destroy = function(self)
		cWordSprite.destroy(self)
		self.chinese:destroy()
	end
	local finish_closure = function()
		local frame = 0
		return function(self)
			for _, v in pairs(self.sp) do
				v:set_alpha(255 - frame * 5)
			end
			self.chinese:set_alpha(255 - frame * 5)
			frame = frame + 1
			if frame == 51 then return true end
		end
	end
	ins.finish = finish_closure()
	for i = 1, #ins.sp do
		ins.frame[i] = math.random(50, word_interval)
	end
	return ins
end

--game_scene
game_scene = {
	enter = function(self)
		userlib.load_dict()

		local font = cFont:new{height = 20, width = 10}
		self.input_ui = cText:new{contents = "input:", font = font}
		self.input_ui:set_rect{l = 50, t = 560, b = 590, r = 750}
		self.input_window = cWindow:new({l = 20, r = 750, t = 550, b = 595 }, -1)
		self.writingspace = cText:new{contents = "", font = font}
		self.writingspace:set_rect{l = 105, t = 560, b = 590, r = 750}
		self.writingspace:set_color(34, 200, 200)

		self.str = ""

		self.word = {}
		self.end_word = {} -- words which reach ends

		self.usertable = userlib.open(user)
		self.nscore = self.usertable.score
		self.wordlist = userlib.sort(self.usertable.list)
		self.cur_word = 0

		self.ncurscore = 0

		self.score = cText:new{contents = "total words answered:"..tostring(self.nscore), font = font}
		self.curscore = cText:new{contents = "score:"..tostring(self.ncurscore), font = font}
		self.curscore:set_rect{l = 0, t = 30, b = 50, r = 800}
	end,
	closure_update = function(self)
	-- declare some internal variables here
	local frame = 0
	local finish_object = {}
	-- then body of function
		return function(self)
			frame = frame + 1
			if frame == word_interval then
				-- get a new word
				self.cur_word = self.cur_word + 1
				local id = self.wordlist[self.cur_word].id
				local t = cWord:new{contents = userlib.dict[id].word, chinese = userlib.dict[id].chinese, x = 200, y = 30}
				table.insert(self.word, t)
				t.info = self.wordlist[self.cur_word]
				frame = 1
			end

			for i, v in ipairs(self.end_word) do
				--decrease the familiarity
				v.info.fam = v.info.fam - 0.01
				if v.info.fam < 0 then v.info.fam = 0 end

				v:update()
				local cury = v:get_y()
				if cury < 550 - i * 50 then
					if not v.drop_frame then v.drop_frame = 0 end
					v.drop_frame = v.drop_frame + 1
					if v.drop_frame >= 51 then
						v:set_y(cury + 25)
					end
					v.drop_frame = v.drop_frame % 52
				end
			end
			for i, v in pairs(self.word) do
				v:update()
				local newy = v:get_y() + word_velocity
				if newy > 500 - #self.end_word * 50 then
					newy = 500 - #self.end_word * 50
					table.remove(self.word, i)
					table.insert(self.end_word, v)
				end
				-- if newy == 0 then end -- gameover
				v:set_y(newy)
			end
			for i, v in pairs(finish_object) do
				if v:finish() then
					table.remove(finish_object, i)
					v:destroy()
				end
			end
			local str = get_input_letter()
			if str then
				self.str = self.str..str
				self.writingspace:set_contents(self.str)
			end
			if input.key_release(input.IK_RETURN) or input.key_release(input.IK_SPACE) then
				for i, v in pairs(self.word) do
					if self.str == v.contents then
						-- right answer!
						v.info.fam = v.info.fam + 1
						v.info.times = v.info.times + 1
						v.info.last_time = os.time()
						table.remove(self.word, i)
						table.insert(finish_object, v)
						self.nscore = self.nscore + 1
						local str = string.format("total words answered:%i", self.nscore)
						self.score:set_contents(str)

						self.ncurscore = self.ncurscore + 10
						str = string.format("score:%i", self.ncurscore)
						self.curscore:set_contents(str)
					end
				end
				for i, v in ipairs(self.end_word) do
					if self.str == v.contents then
						-- right answer!
						v.info.fam = v.info.fam + 1
						v.info.times = v.info.times + 1
						v.info.last_time = os.time()
						table.remove(self.end_word, i)
						table.insert(finish_object, v)
						self.nscore = self.nscore + 1
						local str = string.format("total words answered:%i", self.nscore)
						self.score:set_contents(str)

						self.ncurscore = self.ncurscore + 10
						str = string.format("score:%i", self.ncurscore)
						self.curscore:set_contents(str)
					end
				end
				self.str = ""
				self.writingspace:set_contents("")
			end
			--pause
			if input.key_release(input.IK_ESCAPE) then
				self.pause_ui = cSprite:new{file = "pause.png", z = 200}
				self.pause_ui:set_width(800)
				self.pause_ui:set_height(600)
				self.update = function(self)
					if input.key_release(input.IK_ESCAPE) then
						self.pause_ui:destroy()
						self.pause_ui = nil
						self.update = self.game_update
					end
				end
			end
			--game over
			if #self.end_word == 10 then self.update = self:closure_end() end
		end
	end,
	closure_end = function(self)
		self.gameover = cSprite:new{file = "gameover.png", z = 100 }
		self.gameover:set_width(800)
		self.gameover:set_height(600)
		return function(self)
			if input.key_release(input.IK_R) then
				enter_scene(self) -- restart
				self.update = self.game_update
			end
		end
	end,
	escape = function(self)
		self.usertable.score = self.nscore
		userlib.save(user, self.usertable)
		if self.gameover then self.gameover:destroy(); self.gameover = nil end
		for _, v in pairs(self.end_word) do v:destroy() end
		for _, v in pairs(self.word) do v:destroy() end
		self.input_ui:destroy()
		self.input_window:destroy()
		self.curscore:destroy()
		self.score:destroy()
		self.writingspace:destroy()
	end
}
game_scene.game_update = game_scene:closure_update()
game_scene.update = game_scene.game_update

--initialize scene
scene = title_scene
scene:enter()
function main()
	scene:update()
end
function destroy()
	scene:escape()
end
