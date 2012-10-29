/**
 * @file
 *
 * @brief
 *
 * @date 25.10.2012
 * @author Vita Loginova
 */

#include <embox/test.h>
#include <sys/socket.h>
#include <net/util/request_parser.h>

EMBOX_TEST_SUITE("network/parser/request_parser test");

TEST_CASE("Parse HTTP methods") {
	char * test_get = "GET /wiki/index.html HTTP/1.1";
	char * test_head = "HEAD /wiki/index.html HTTP/1.1";
	char * test_put = "PUT /wiki/index.html HTTP/1.1";

	char method[20] = "";
	http_request parsed_request;
	parsed_request.method = method;

	parse_http(test_get, &parsed_request);
	test_assert_str_equal(parsed_request.method, "GET");

	strcpy(parsed_request.method , "");
	parse_http(test_head, &parsed_request);
	test_assert_str_equal(parsed_request.method, "HEAD");

	strcpy(parsed_request.method , "");
	parse_http(test_put, &parsed_request);
	test_assert_str_not_equal(parsed_request.method, "PUT");
}

TEST_CASE("Parse first string") {
	char *test_get = "HEAD 10.10.111.1:80 HTTP/1.1";
	char *test_get2 = "GET /~first_folder/second/index.html HTTP/1.1";
	char *test_get3 = "GET 10.10.111.1:80/~first_folder/second/index.html#frag01 HTTP/1.1";
	char *test_get4 = "GET wsdf-365.ru/first_folder/index.php?p1+p2&p3=54; HTTP/1.1";

	test_assert_true(check_firstline(test_get));
	test_assert_true(check_firstline(test_get2));
	test_assert_true(check_firstline(test_get3));
	test_assert_true(check_firstline(test_get4));

}
