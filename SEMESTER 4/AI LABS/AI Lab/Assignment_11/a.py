from selenium import webdriver
from selenium.webdriver.common.keys import Keys
import time

# Function to delete messages before a specified date
def delete_messages_before_date(driver, date):
    # Click on the chat box
    chat_box = driver.find_element_by_xpath('//div[@contenteditable="true"][@data-tab="6"]')
    chat_box.click()
    time.sleep(2)

    # Get all the messages
    messages = driver.find_elements_by_xpath('//div[contains(@class, "message-in")]')

    # Loop through the messages
    for message in messages:
        # Get the message timestamp
        timestamp_element = message.find_element_by_xpath('.//div[contains(@class, "copyable-text")]//span[contains(@class, "_3EFt_")]/span')
        timestamp = timestamp_element.get_attribute('title')

        # Convert timestamp to date
        message_date = timestamp.split(',')[0]

        # If the message date is before the specified date, delete the message
        if message_date < date:
            message.click()
            time.sleep(1)
            # Click on the delete button
            delete_button = driver.find_element_by_xpath('//div[@class="_3e4VU"]')
            delete_button.click()
            time.sleep(1)
            # Confirm delete
            confirm_delete_button = driver.find_element_by_xpath('//div[@class="_3s1D4"]')
            confirm_delete_button.click()
            time.sleep(1)

# Initialize the Chrome driver (you can use other browsers as well)
driver = webdriver.Chrome()

# Open WhatsApp Web
driver.get("https://web.whatsapp.com/")
time.sleep(1)  # Waiting for the user to scan the QR code and log in

# Prompt user to enter the name of the chat
chat_name = input("Enter the name of the chat: ")

# Locate the chat search box and enter the chat name
search_box = driver.find_element_by_xpath('//div[contains(@class, "_2_1wd")]//input')
search_box.send_keys(chat_name)
search_box.send_keys(Keys.RETURN)

# Prompt user to enter the date before which messages should be deleted
delete_date = input("Enter the date before which messages should be deleted (format: YYYY-MM-DD): ")

# Call function to delete messages before the specified date
delete_messages_before_date(driver, delete_date)

# Close the browser
driver.quit()
