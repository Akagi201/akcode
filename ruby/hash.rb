numbers = {1 => 'one', 2 => 'two'}

numbers[1]

numbers[2]

stuff = {:array => [1, 2, 3], :string => 'Hi, mom!'}

stuff[:string]

def tell_the_truth(options={})
	if options[:profession] == :lawyer
		'it could be believed that this is almost certainly not false.'
	else
		true
	end
end

tell_the_truth

tell_the_truth :profession => :lawyer

