#pragma once

//SFINAE : https://en.wikipedia.org/wiki/Substitution_failure_is_not_an_error

namespace ecs
{
    template <typename T>
    struct has_free {
        typedef char yes[1];
        typedef char no[2];

        template <typename C>
        static yes& detect(decltype(&C::free));

        template <typename>
        static no& detect(...);

        static const bool result = sizeof(detect<T>(nullptr)) == sizeof(yes);
    };
};