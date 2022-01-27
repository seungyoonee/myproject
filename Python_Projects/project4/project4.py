def read_market_data(filename):
    markets_file = open(filename, 'r')
    
    zip_to_market_dict = {}
    for line in markets_file:
        line = line.split("@")

        if line == ['']:
            break

        if line[4] in zip_to_market_dict:
            zip_to_market_dict[line[4]].append((line[0], line[1], line[2], line[3], line[4]))
        else:
            zip_to_market_dict[line[4]] = ([(line[0], line[1], line[2], line[3], line[4])])
            
    markets_file.close()

    markets_file = open(filename, 'r')

    town_to_zip_dict = {}
    for line in markets_file:
        line = line.split("@")
    
        if line == ['']:
            break
        
        if line[4] == '' or line[3] == '':
            continue
        elif line[3] in town_to_zip_dict:
            town_to_zip_dict[line[3]].add(line[4])
        else:
            town_to_zip_dict[line[3]] = {line[4]}

    markets_file.close()
    
    return zip_to_market_dict, town_to_zip_dict

def print_market_data(market):
    formatted_string = ""
    
    for x in range(len(market)):
        market_name = str(market[x][1])
        street_name = str(market[x][2])
        town_name = str(market[x][3])
        state_name = str(market[x][0])
        zip_code = str(market[x][4])
        temp_string = "\n{}\n{}\n{}, {} {}\n".format(market_name, street_name, town_name, state_name, zip_code)
        formatted_string += temp_string
    
    return formatted_string

if __name__ == "__main__":
    # This main program first reads in the markets.txt once (using the function
    # from part (a)), and then asks the user repeatedly to enter a zip code or
    # a town name (in a while loop until the user types "quit").
    FILENAME = "markets.txt"
    try: 
        zip_to_market, town_to_zips = read_market_data(FILENAME)
        
        user_input = input(("Please enter a town or a zipcode to search for farmer's markets:\n"))
        while user_input != "quit":
            if user_input.isnumeric():
                if str(user_input) in zip_to_market:
                    print(print_market_data(zip_to_market[user_input]), end='')
                else:
                    print("Not found.")
            else:
                user_input = user_input.title()
                if user_input in town_to_zips:
                    for x in town_to_zips[user_input]:
                        print(print_market_data(zip_to_market[x]), end='')
                else:
                    print("Not found.")
                    
            user_input = input(("Please enter a town or a zipcode to search for farmer's markets:\n"))

    except (FileNotFoundError, IOError): 
        print("Error reading {}".format(FILENAME))

