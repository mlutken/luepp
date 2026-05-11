#pragma once

#include <cstdint>
#include <string>
#include <vector>
// #include <memory>
// #include <unordered_map>
#include <variant>
// #include <functional>

#include "data/data_path.h"
#include "data/data_object_base.h"
#include "data/data_value.h"


namespace lue::data {

// class data_value;


// // ------------------
// // --- data_value ---
// // ------------------
// class data_source_base;
// using data_source_base_sp   = std::shared_ptr<data_source_base>;

// using data_value            = std::variant<int32_t, int64_t, float, double, bool, std::string,
//                                            data_object_sp, data_source_base_sp>;
// using data_value_vec        = std::vector<data_value>;

// std::string     to_string   (const data_value& val);
// std::string     to_string   (const data_value_vec& val);

// ------------------------
// --- data_source_base ---
// ------------------------

/**
@todo Most likely we do not want data_source_base to be copyable ...
 */
class data_source_base
{
public:
    data_source_base()                                      = default;
    virtual ~data_source_base()                             = default;
    data_source_base(const data_source_base&)               = default;
    data_source_base(data_source_base&&)                    = default;
    data_source_base& operator=(const data_source_base&)    = default;
    data_source_base& operator=(data_source_base&&)         = default;
    explicit data_source_base   (data_path);


    void                        set                 (const data_path& path, data_value val);
    void                        set                 (const data_path& path, data_value_vec val);
    void                        set_data_source     (const data_path& path, data_source_base_sp val);

    int32_t                     as_int32            (const data_path& path) const;
    const std::string&          as_string           (const data_path& path)  const;
    const data_value&           as_data_value       (const data_path& path)  const;

    void                        connect             (data_changed_cb cb);

    bool                        is_read_only        () const;

    const data_path&            path                () const    { return path_; }
    std::string                 to_string           () const;
    void                        dbg_print           () const;

protected:
    virtual const data_value&   do_as_data_value    (const data_path& path)  const = 0;
    virtual void                do_set              (const data_path& path, data_value val) = 0;
    virtual void                do_set_data_vec     (const data_path& path, data_value_vec val) = 0;
    virtual void                do_set_data_source  (const data_path& path, data_source_base_sp val) = 0;
    virtual std::string         do_to_string        () const = 0;

    virtual bool                do_is_read_only     () const { return false; }

private:
    using data_changed_cb_vec_t = std::vector<data_changed_cb>;

    void                        call_on_changed     (const data_path& path) const;

    data_path                   path_                   {};
    data_changed_cb_vec_t       data_changed_cb_vec_    {};
};

} // namespace lue::data
