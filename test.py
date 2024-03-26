x = input( "" ).replace( "────────", "" )
output = "{\n"

for bigLoop in range( 1 ):
    
    y = x[ 48 * bigLoop : 48 * (bigLoop + 1) ]
    
    output += "    {\n        {\n"
    
    for loop in range( 6 ):
        
        z = y[ 8 * loop : 8 * ( loop + 1 ) ]
        
        output += "            { "
        
        for smallLoop in range( 8 ):
            output += f"'{ z[ smallLoop : smallLoop + 1 ] }', "
        
        output = output[ 0 : -2 ] + ", '\\0' },\n"
    
    output = output[ 0 : -2 ] + "\n        }\n    },\n"

output = output[ 0 : -2 ] + "\n};"

print( output.replace( "'▄'", "-36" ).replace( "'█'", "-37" ).replace( "'▀'", "-33" ) )
input()

