
-- Formatted printing.
-- An implementation of printf

function printf(fmt, ...)
	io.write(string.format(fmt, ...))
end

printf("Hello %s from %s on %s\n",
	os.getenv"USER" or "there", _VERSION, os.date())
	