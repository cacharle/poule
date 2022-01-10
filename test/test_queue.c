#include "poule/queue.h"
#include <criterion/criterion.h>

Test(queue, init)
{
    cr_assert_str_empty("a");
}
