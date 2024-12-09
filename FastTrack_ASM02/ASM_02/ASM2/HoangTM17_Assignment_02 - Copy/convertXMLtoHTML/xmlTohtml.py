import sys
import os
import shutil
import glob
import math
import xml.etree.ElementTree as ET
from templates.html_templates import *
# Importing required libraries:
# - sys: for system-specific parameters and functions.
# - os: for interacting with the operating system (file/directory handling).
# - shutil: for file operations like copying or removing files.
# - glob: for file pattern matching and retrieving file paths.
# - math: for mathematical operations.
# - xml.etree.ElementTree (ET): for parsing XML reports.
# - html_templates: presumably a module with HTML templates used for report generation.

# Template scheme.
# -> tmpl_main_html[]
# This comment likely refers to a structure or pattern used in the HTML templates (tmpl_main_html[]),
# probably defining the layout or structure of the HTML output.

def usage():
    print('Usage:')
    print('  python Gtest2Html.py <REPORT_FILE> <OUTPUT_FILE>')
    print('  Args:')
    print('    REPORT_FILE: Gtest xml report.')
    print('    OUTPUT_FILE: Path to the output file, e.g. "index.html"')
# The `usage()` function prints instructions for how to run the script.
# It explains that the script converts a Google Test (Gtest) XML report into an HTML file.

def error_gen(actual, rounded):
    divisor = math.sqrt(1.0 if actual < 1.0 else actual)
    return abs(rounded - actual) ** 2 / divisor
# The `error_gen` function computes the "error" between two numbers (actual and rounded),
# scaled by the square root of the actual value (to reduce the impact of large values).
# It is used later to adjust percentages in the `round_to_100` function.

def round_to_100(percents):
    if not math.isclose(sum(percents), 100):
        raise ValueError
    # Check if the sum of the percentages is close to 100. If not, raise a ValueError.

    n = len(percents)
    # Get the number of percentages.

    rounded = [int(x) for x in percents]
    # Round each percentage down to the nearest integer.

    up_count = 100 - sum(rounded)
    # Calculate how many more units need to be added to make the total sum exactly 100.

    errors = [(error_gen(percents[i], rounded[i] + 1) - error_gen(percents[i], rounded[i]), i)
              for i in range(n)]
    # Compute the difference in error between rounding a percentage up by 1 and not rounding it up,
    # for each percentage. This difference is used to rank which percentages should be incremented.

    rank = sorted(errors)
    # Sort the errors to determine which percentages should be incremented.

    for i in range(up_count):
        rounded[rank[i][1]] += 1
    # Increment the percentages with the smallest error, up to the required count (`up_count`).

    return rounded
    # Return the final list of rounded percentages that sum to exactly 100.

def check_for_unkown_attributes(xml_node, known_attributes, include_empty_attributes=False):
    # Print warning for each unknown attribute inside the xml node.
    # - xml_node: the XML node to check.
    # - known_attributes: a list of valid or expected attributes for this node.
    # - include_empty_attributes: if True, include attributes with empty values in the warning.

    for unknown_attribute in list(filter(lambda x: x not in known_attributes, xml_node.attrib)):
        # For each attribute in the XML node's attributes (xml_node.attrib),
        # check if it is NOT in the list of known attributes.
        # `filter` is used to get only unknown attributes, and `list` makes it iterable.

        if not xml_node.attrib[unknown_attribute].strip() or include_empty_attributes:
            # Check if the unknown attribute's value is either empty (strip removes whitespace)
            # or if the include_empty_attributes flag is set to True.

            print('Warning: Unknown attribute {!r} found in node {!s}[{!r}] which is not parsed.'.format(
                unknown_attribute, xml_node.tag, xml_node.attrib))
            # Print a warning with details of the unknown attribute:
            # - {unknown_attribute}: the name of the unknown attribute.
            # - {xml_node.tag}: the tag name of the current XML node.
            # - {xml_node.attrib}: the entire dictionary of attributes for this node.

def generate_progress_bars(abs_total, abs_success, abs_fail, abs_disabled):
    html_progressbars = ''
    # Initialize an empty string to hold the generated HTML progress bars.

    if abs_total > 0:
        # If the total number of test cases (abs_total) is greater than 0, proceed to generate progress bars.

        html_class_list = ['success', 'danger', 'warning']
        # Define the CSS classes for the different states:
        # - 'success': for successful tests.
        # - 'danger': for failed tests.
        # - 'warning': for disabled tests.

        abs_value_list = [abs_success, abs_fail, abs_disabled]
        # List of absolute values representing:
        # - Successful tests (abs_success).
        # - Failed tests (abs_fail).
        # - Disabled tests (abs_disabled).

        percentage_list = list(map(lambda x: 100.0 * x / float(abs_total), abs_value_list))
        # Calculate the percentage for each of the success, fail, and disabled tests.
        # Each percentage is relative to the total number of tests (abs_total).

        rounded_percentage_list = round_to_100(percentage_list)
        # Use the `round_to_100` function to ensure that the sum of the percentages is exactly 100.

        for idx in range(len(abs_value_list)):
            # Iterate through each test case result (success, fail, disabled).

            abs_value = abs_value_list[idx]
            # Get the absolute value of the current result (success, fail, or disabled).

            percentage_rate = rounded_percentage_list[idx]
            # Get the rounded percentage for the current result.

            html_class = html_class_list[idx]
            # Get the corresponding CSS class for the current result type.

            if abs_value == 0:
                continue
            # If the number of tests for this category is 0, skip adding the progress bar.

            html_progressbars += tmpl_progress_bar.format(
                html_class=html_class,
                percentage_rate=percentage_rate,
                absolute_value=abs_value
            )
            # Generate an HTML progress bar using the template `tmpl_progress_bar`.
            # Insert the CSS class, percentage, and absolute value into the template.

    else:
        html_progressbars = tmpl_progress_bar.format(
            html_class='success',
            percentage_rate=100,
            absolute_value=0
        )
        # If there are no test cases (abs_total == 0), create a default progress bar
        # with 100% success (even though no tests were run).

    return html_progressbars
    # Return the generated HTML string containing the progress bars.



def get_xml_attribute(convert_func, xml_node, attribute_name, default_value, print_warning=True):
    # This function retrieves an attribute from an XML node.
    # - convert_func: a function to convert the attribute value (e.g., to int, float, etc.).
    # - xml_node: the XML element containing the attribute.
    # - attribute_name: the name of the attribute to retrieve.
    # - default_value: the value to return if the attribute is not found.
    # - print_warning: if True, print a warning when the attribute is missing.

    if not attribute_name in xml_node.attrib and print_warning:
        # Check if the attribute is not present in the XML node's attributes (`xml_node.attrib`).
        # If `print_warning` is True, print a warning message about the missing attribute.

        print('Warning: Attribute {!r} was not found inside xml node {!s}[{!r}]. Set it to its default value {!r}.'.format(
            attribute_name, xml_node.tag, xml_node.attrib, default_value))
        # Print a warning message, providing:
        # - {attribute_name}: the missing attribute.
        # - {xml_node.tag}: the XML node's tag.
        # - {xml_node.attrib}: the node's entire attribute dictionary.
        # - {default_value}: the default value being used as a substitute.

        return default_value
        # If the attribute is not found, return the provided default value.

    return convert_func(xml_node.attrib[attribute_name])
    # If the attribute is found, use the `convert_func` to convert its value (e.g., string to int)
    # and return the converted value.

def generate_total_test_summary(xml_testsuites_node):
    # This function generates a summary of the test results from the root XML <testsuites> node.
    # It parses attributes from the node, generates progress bars, and prepares HTML content
    # summarizing the results.

    # Parse the testsuites node attributes.
    total_abs_test_count = get_xml_attribute(int, xml_testsuites_node, 'tests', 0)
    # Retrieve the total number of tests from the 'tests' attribute and convert it to an integer.
    # If not present, use 0 as the default value.

    total_abs_fail_count = get_xml_attribute(int, xml_testsuites_node, 'failures', 0)
    # Retrieve the total number of failed tests from the 'failures' attribute, with a default of 0.

    total_abs_disabled_count = get_xml_attribute(int, xml_testsuites_node, 'disabled', 0)
    # Retrieve the total number of disabled tests from the 'disabled' attribute, with a default of 0.

    total_abs_success_count = total_abs_test_count - total_abs_fail_count - total_abs_disabled_count
    # Calculate the number of successful tests by subtracting the failures and disabled tests from the total.

    total_execution_time = get_xml_attribute(str, xml_testsuites_node, 'time', 0)
    # Retrieve the total execution time from the 'time' attribute, defaulting to 0 if not present.

    test_timestamp = get_xml_attribute(str, xml_testsuites_node, 'timestamp', 0)
    # Retrieve the timestamp from the 'timestamp' attribute, defaulting to 0 if not present.

    # Kiểm tra và thêm thuộc tính 'project' nếu chưa có
    # Check and add the 'project' attribute if it's missing
    test_project_name = xml_testsuites_node.attrib.get('project', 'Assignment 02: Automation Test Framework')
    # Use the 'project' attribute if available, otherwise set the default project name to 'Assignment 02: Automation Test Framework'.

    xml_testsuites_node.set('project', test_project_name)
    # Set the 'project' attribute in the XML node to the determined value.

    # Kiểm tra và thêm thuộc tính 'author' nếu chưa có
    # Check and add the 'author' attribute if it's missing
    test_author = xml_testsuites_node.attrib.get('author', 'Minh Hoang Tran - HoangTM17')
    # Use the 'author' attribute if available, otherwise default to 'Minh Hoang Tran - HoangTM17'.

    xml_testsuites_node.set('author', test_author)
    # Set the 'author' attribute in the XML node to the determined value.

    testsuite_name = get_xml_attribute(str, xml_testsuites_node, 'name', 'undefined')
    # Retrieve the 'name' attribute of the test suite, defaulting to 'undefined' if not present.

    # Print warning for each unknown attribute inside the node testsuites.
    check_for_unkown_attributes(xml_testsuites_node, ['tests', 'failures',
                                                      'disabled', 'time', 'timestamp', 'author', 'name', 'project'])
    # Call the function to check for any unknown attributes in the <testsuites> node.
    # It checks against a list of expected attributes and prints warnings for any unknown ones.

    # Generate HTML for the navigation bar.
    test_navbar = tmpl_test_navbar.format(
        project_name=test_project_name
    )
    # Use the HTML template `tmpl_test_navbar` to generate the navigation bar,
    # filling in the project name.

    # Generate HTML for the progress bar for the test summary.
    total_test_result_progressbars = generate_progress_bars(
        abs_total=total_abs_test_count,
        abs_success=total_abs_success_count,
        abs_fail=total_abs_fail_count,
        abs_disabled=total_abs_disabled_count
    )
    # Generate progress bars using the `generate_progress_bars` function.
    # It creates bars based on the number of total, success, fail, and disabled tests.

    # Generate HTML for the test summary.
    total_test_result = tmpl_total_test_result.format(
        report_file_path=os.path.basename(report_file),
        # Get the base name of the report file to include in the summary.

        testsuite_name=testsuite_name,
        # Include the name of the test suite.

        total_abs_test_count=total_abs_test_count,
        # Include the total number of tests.

        total_abs_success_count=total_abs_success_count,
        # Include the total number of successful tests.

        total_abs_fail_count=total_abs_fail_count,
        # Include the total number of failed tests.

        total_abs_disabled_count=total_abs_disabled_count,
        # Include the total number of disabled tests.

        html_progress_bars=total_test_result_progressbars,
        # Include the generated progress bars in the summary.

        total_execution_time=total_execution_time,
        # Include the total execution time for the tests.

        test_timestamp=test_timestamp,
        # Include the timestamp for when the tests were run.

        test_author=test_author
        # Include the author of the test suite.
    )
    # Use the template `tmpl_total_test_result` to generate the complete HTML for the test result summary,
    # filling in all relevant test data and formatting it properly.

    return test_navbar, total_test_result_progressbars, total_test_result
    # Return the generated HTML components: the navigation bar, progress bars, and the test result summary.


def generate_single_testcase_rows(xml_testsuite_node):
    # This function generates HTML rows for each test case in a given <testsuite> XML node.
    # It processes each <testcase> element and formats it into an HTML row for display.

    html_single_testcase_rows = ''
    # Initialize an empty string to accumulate HTML rows for all test cases.

    xml_testcase_nodes = xml_testsuite_node.findall('./testcase')
    # Find all <testcase> nodes within the <testsuite> node.

    if len(xml_testcase_nodes) == 0:
        print("Warning: No nodes {!r} found in testsuite element with name {!r}.".format(
            'testcase', test_name))
        # If no <testcase> nodes are found, print a warning. Note that `test_name` needs to be defined or retrieved from the context.

    for idx, xml_testcase_node in enumerate(xml_testcase_nodes):
        # Iterate over each <testcase> node, with `idx` being the index of the test case.

        test_number = idx + 1
        # Assign a sequential number to the test case, starting from 1.

        test_name = get_xml_attribute(str, xml_testcase_node, 'name', '-undefined-')
        # Retrieve the 'name' attribute of the test case, defaulting to '-undefined-' if not present.

        test_execution_time = get_xml_attribute(str, xml_testcase_node, 'time', '-undefined-')
        # Retrieve the 'time' attribute of the test case, defaulting to '-undefined-' if not present.

        test_status = get_xml_attribute(str, xml_testcase_node, 'status', '-undefined-')
        # Retrieve the 'status' attribute of the test case, defaulting to '-undefined-' if not present.

        test_classname = get_xml_attribute(str, xml_testcase_node, 'classname', '-undefined-')
        # Retrieve the 'classname' attribute of the test case, defaulting to '-undefined-' if not present.

        # Kiểm tra và thêm thuộc tính 'tags' nếu chưa có
        # Check and add the 'tags' attribute if it's missing
        if 'tags' not in xml_testcase_node.attrib:
            xml_testcase_node.set('tags', '')
        # Set the 'tags' attribute to an empty string if it is not present in the XML node.

        test_tags = xml_testcase_node.attrib['tags']
        # Retrieve the 'tags' attribute from the test case.

        test_icon_name = ''
        test_html_class = 'primary'
        html_error_message_list = ''
        # Initialize variables for the test case's icon name, HTML class, and error message list.

        # Print warning for each unknown attribute inside the node testcase.
        check_for_unkown_attributes(
            xml_testcase_node, ['name', 'time', 'status', 'classname', 'tags'])
        # Check for any unknown attributes in the <testcase> node and print warnings if found.

        # Select icon name and HTML class considering the number of failure-children and the test status.
        xml_failure_nodes = xml_testcase_node.findall('./failure')
        # Find all <failure> nodes within the current <testcase> node.

        if len(xml_failure_nodes) == 0 and test_status == 'run':
            test_icon_name = 'check'
            test_html_class = 'success'
        elif test_status == 'notrun':
            test_icon_name = 'warning'
            test_html_class = 'warning'
        else:
            test_icon_name = 'x'
            test_html_class = 'danger'
        # Determine the icon and HTML class based on the presence of <failure> nodes and the test status:
        # - No failures and status 'run' -> success (check icon)
        # - Status 'notrun' -> warning (warning icon)
        # - All other cases -> danger (error icon)

        # If failures occur, generate the listing with error messages.
        if len(xml_failure_nodes) > 0:
            html_error_message_items = ''
            for xml_failure_node in xml_failure_nodes:
                error_message = get_xml_attribute(
                    str, xml_failure_node, 'message', '-undefined-')
                # Retrieve the 'message' attribute from each <failure> node, defaulting to '-undefined-' if not present.

                error_type = get_xml_attribute(str, xml_failure_node, 'type', '-undefined-')
                # Retrieve the 'type' attribute from each <failure> node, defaulting to '-undefined-' if not present.
                error_type = '' if not error_type else ' (type = {})'.format(error_type)
                # Format the error type if it exists.

                # Print warning for each unknown attribute inside the node failure.
                check_for_unkown_attributes(xml_failure_node, ['message', 'type'])
                # Check for any unknown attributes in the <failure> node and print warnings if found.

                html_error_message_items += tmpl_error_message_item.format(
                    error_message=error_message,
                    error_type=error_type
                )
                # Append the formatted error message item to the list.

            html_error_message_list = tmpl_error_message_listing.format(
                html_error_message_items=html_error_message_items
            )
            # Generate the complete HTML for the error message listing.

        # Create the HTML code for this single testcase.
        html_single_testcase_rows += tmpl_single_test_row.format(
            test_number=test_number,
            test_classname=test_classname,
            test_name=test_name,
            test_tags=test_tags,
            html_error_message_list=html_error_message_list,
            test_execution_time=test_execution_time,
            test_icon_name=test_icon_name,
            test_html_class=test_html_class
        )
        # Format and append the HTML for this individual test case to the accumulating string.

    return html_single_testcase_rows
    # Return the complete HTML string with rows for all test cases.


def generate_single_test_result_listings(xml_testsuites_node):
    # This function generates HTML listings for individual test results from a <testsuites> XML node.
    # It processes each <testsuite> element and generates HTML to display the results of each test suite.

    html_single_test_result_listing = ''
    # Initialize an empty string to accumulate HTML listings for all test suites.

    collected_testsuite_ids = []
    # Initialize a list to keep track of test suite names and their corresponding HTML IDs.

    # Generate HTML for the single test result listings.
    xml_testsuite_nodes = xml_testsuites_node.findall('./testsuite')
    # Find all <testsuite> nodes within the <testsuites> node.

    if len(xml_testsuite_nodes) == 0:
        print('Warning: No nodes {!r} found in {!r}. Nothing is listed inside the single test_result listing.'.format(
            'testsuite', report_file))
        # If no <testsuite> nodes are found, print a warning. Note that `report_file` needs to be defined or retrieved from the context.

    id_counter = 0
    # Initialize a counter to assign unique IDs to each test suite in the HTML output.

    for xml_testsuite_node in xml_testsuite_nodes:
        # Iterate over each <testsuite> node.

        # Parse XML attributes.
        testsuite_name = get_xml_attribute(str, xml_testsuite_node, 'name', '-undefined-')
        # Retrieve the 'name' attribute of the test suite, defaulting to '-undefined-' if not present.

        testsuite_abs_test_count = get_xml_attribute(int, xml_testsuite_node, 'tests', 0)
        # Retrieve the 'tests' attribute of the test suite, defaulting to 0 if not present.

        testsuite_abs_fails_count = get_xml_attribute(int, xml_testsuite_node, 'failures', 0)
        # Retrieve the 'failures' attribute of the test suite, defaulting to 0 if not present.

        testsuite_abs_disabled_count = get_xml_attribute(int, xml_testsuite_node, 'disabled', 0)
        # Retrieve the 'disabled' attribute of the test suite, defaulting to 0 if not present.

        testsuite_abs_success_count = testsuite_abs_test_count - \
            testsuite_abs_fails_count - testsuite_abs_disabled_count
        # Calculate the number of successful tests.

        testsuite_execution_time = get_xml_attribute(
            str, xml_testsuite_node, 'time', '-undefined-')
        # Retrieve the 'time' attribute of the test suite, defaulting to '-undefined-' if not present.

        # Kiểm tra và thêm thuộc tính 'tags' nếu chưa có
        # Check and add the 'tags' attribute if it's missing
        if 'tags' not in xml_testsuite_node.attrib:
            xml_testsuite_node.set('tags', '')
        # Set the 'tags' attribute to an empty string if it is not present in the XML node.

        testsuite_tags = xml_testsuite_node.attrib['tags']
        # Retrieve the 'tags' attribute from the test suite.

        # Print warning for each unknown attribute inside the node testsuite.
        check_for_unkown_attributes(xml_testsuite_node, ['name', 'tests',
                                                         'failures', 'disabled', 'time', 'tags'])
        # Check for any unknown attributes in the <testsuite> node and print warnings if found.

        # Generate HTML for single test cases within this test suite.
        html_single_testcase_rows = generate_single_testcase_rows(xml_testsuite_node)
        # Call `generate_single_testcase_rows` to get the HTML for individual test cases in the test suite.

        # Generate HTML for progress bars for this test suite.
        html_testsuites_progress_bars = generate_progress_bars(
            abs_total=testsuite_abs_test_count,
            abs_success=testsuite_abs_success_count,
            abs_fail=testsuite_abs_fails_count,
            abs_disabled=testsuite_abs_disabled_count
        )

        # Generate the HTML for this test suite.
        html_testsuite = tmpl_single_test_result_listing.format(
            html_progress_bars=html_testsuites_progress_bars,
            testsuite_name=testsuite_name,
            testsuite_tags=testsuite_tags,
            testsuite_html_id=id_counter,
            testsuite_abs_test_count=testsuite_abs_test_count,
            testsuite_abs_success_count=testsuite_abs_success_count,
            testsuite_abs_fails_count=testsuite_abs_fails_count,
            testsuite_abs_disabled_count=testsuite_abs_disabled_count,
            testsuite_execution_time=testsuite_execution_time,
            html_single_test_rows=html_single_testcase_rows
        )
        # Format the HTML template for this test suite with the collected data.

        collected_testsuite_ids.append((testsuite_name, id_counter))
        # Append the test suite name and its HTML ID to the collected list.

        id_counter += 1
        # Increment the ID counter for the next test suite.

        html_single_test_result_listing += html_testsuite
        # Append the formatted HTML for this test suite to the accumulating string.

    return html_single_test_result_listing, collected_testsuite_ids
    # Return the complete HTML string with listings for all test suites and the collected IDs.

def generate_test_sidebar(collected_testsuite_ids):
    # This function generates the HTML for a sidebar that lists links to individual test suites.
    # It uses the list of test suite IDs to create navigation links in the sidebar.

    html_single_testsuite_links = ''
    # Initialize an empty string to accumulate HTML links for each test suite.

    for name_id_pair in collected_testsuite_ids:
        # Iterate over the list of collected test suite names and IDs.
        # Each pair contains the name and the corresponding HTML ID for the test suite.

        html_single_testsuite_links += tmpl_single_testsuite_link.format(
            testsuite_name=name_id_pair[0],
            testsuite_html_id=name_id_pair[1]
        )
        # For each test suite, format the HTML link using the name and ID from the pair.
        # Append the formatted link to the `html_single_testsuite_links` string.

    html_test_sidebar = tmpl_test_sidebar.format(
        single_testsuite_links=html_single_testsuite_links
    )
    # Format the sidebar template with the accumulated links.

    return html_test_sidebar
    # Return the complete HTML for the sidebar, including all test suite links.


def generate_html(report_file, destination_file):
    # This function generates an HTML report from a given XML report file and saves it to a specified destination file.

    # Parse XML.
    xml_tree = ET.parse(report_file)
    xml_root = xml_tree.getroot()
    # Load and parse the XML report file, and get the root element of the XML tree.

    # Check if the root element 'testsuites' exists.
    xml_testsuites_nodes = xml_root.findall('.')
    if len(xml_testsuites_nodes) == 0:
        print('Error: The xml file {!r} has no root node.'.format(report_file))
        exit(-1)
    # Ensure that the XML file contains at least one element.
    
    xml_testsuites_node = xml_testsuites_nodes[0]
    if xml_testsuites_node.tag != 'testsuites':
        print('Error: The xml file {!r} has an invalid root node tag (found: {!r}, expected: {!r})'.format(
            report_file, xml_testsuites_node.tag, 'testsuites'))
        exit(-1)
    # Verify that the root element is 'testsuites'. If not, print an error message and exit.

    # Generate the HTML content.
    html_single_test_result_listing, collected_testsuite_ids = generate_single_test_result_listings(
        xml_testsuites_node)
    # Create HTML for individual test results and collect test suite IDs for the sidebar.

    test_navbar, total_test_result_progressbars, total_test_result = generate_total_test_summary(
        xml_testsuites_node)
    # Generate HTML for the navigation bar, total test result summary, and progress bars.

    test_sidebar = generate_test_sidebar(collected_testsuite_ids)
    # Generate HTML for the sidebar navigation links.

    html_code = tmpl_main_html.format(
        test_navbar=test_navbar,
        test_sidebar=test_sidebar,
        total_test_result=total_test_result,
        single_test_result_listing=html_single_test_result_listing
    )
    # Format the main HTML template with the generated HTML snippets for the navigation bar, sidebar, test results, and summary.

    # Write the generated HTML to the destination file.
    with open(destination_file, 'w') as fout:
        fout.write(html_code)

    return True
    # Return True to indicate that the HTML file was successfully generated.


if __name__ == '__main__':
    # This block of code runs when the script is executed directly.

    if len(sys.argv) < 2:
        usage()
        exit(0)
    # Check if at least one argument (the report file) is provided.
    # If not, print usage instructions and exit.

    # Get the source and destination directories.
    source_directory = os.path.dirname(os.path.realpath(__file__))
    destination_directory = os.path.dirname(sys.argv[2])
    report_file = os.path.realpath(sys.argv[1])
    destination_file = os.path.realpath(sys.argv[2])
    # Define the source directory (where the script is located) and
    # resolve the absolute paths for the report and destination files.

    if not os.path.exists(report_file):
        print('ERROR: The report file {} does not exist.'.format(report_file))
        usage()
        exit(1)
    # Check if the report file exists. If not, print an error message,
    # display usage instructions, and exit.

    # Create the destination directory if not exists.
    if not os.path.isdir(destination_directory):
        os.makedirs(destination_directory)
    # Ensure that the destination directory exists. Create it if it doesn't.

    # Copy files from html_resources.
    resource_files = glob.glob(os.sep.join([source_directory, 'html_resources/*']))
    for rs in resource_files:
        # Copy all files from the 'html_resources' directory to the destination directory.
        if os.path.isfile(rs):
            shutil.copy(rs, destination_directory)
        else:
            dirname = os.path.split(rs)[-1]
            target = os.sep.join([destination_directory, dirname])

            if os.path.exists(target):
                shutil.rmtree(target)
            shutil.copytree(rs, os.sep.join([destination_directory, dirname]))
    # Copy HTML resource files (both files and directories) from the source directory to the destination directory.
    # Remove existing directories if they exist before copying new ones.

    # Generate html.
    print('Start generation:')
    print('  input  : {}'.format(os.path.basename(report_file)))
    print('  output : {}'.format(os.path.basename(destination_file)))
    if generate_html(report_file, destination_file):
        print('Html was generated successfully.')
    # Print the input and output file names, call the `generate_html` function to create the HTML report,
    # and print a success message if the HTML report was generated successfully.
