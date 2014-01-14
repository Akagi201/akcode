3.times {puts 'hiya there, kiddo'}

animals = ['lions and ', 'tigers and', 'bears', 'oh my']

animals.each {|a| puts a}

class Fixnum
	def my_times
		i = self
		while i > 0
			i = i - 1
			yield
		end
	end
end

3.my_times {puts 'mangy moose'}

def call_block(&block)
	block.call
end

def pass_block(&block)
	call_block(&block)
end

pass_block {puts 'Hello, block'}


