#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "data/data_path.h"
#include "data/data_value.h"


namespace lue::data {


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
    void                        set                 (std::string_view path, data_value val);
    void                        set                 (const data_path& path, data_value_vec val);
    void                        set                 (std::string_view path, data_value_vec val);
    void                        set_data_source     (const data_path& path, data_source_base_sp val);

    int32_t                     as_int32            (const data_path& path) const;
    const std::string&          as_string           (const data_path& path)  const;
    const data_value&           as_data_value       (const data_path& path)  const;

    void                        connect             (data_changed_cb cb);

    bool                        is_read_only        () const;
    bool                        is_valid            () const;

    const data_path&            path                () const    { return path_; }
    std::string                 to_string           () const;
    void                        dbg_print           () const;

protected:
    virtual const data_value&   do_as_data_value    (const data_path& path)  const = 0;
    virtual void                do_set              (const data_path& path, data_value val) = 0;
    virtual void                do_set_data_vec     (const data_path& path, data_value_vec val) = 0;
    virtual void                do_set_data_source  (const data_path& path, data_source_base_sp val) = 0;
    virtual std::string         do_to_string        () const = 0;

    virtual bool                do_is_read_only     () const { return false;    }
    virtual bool                do_is_valid         () const { return true;     }

private:
    using data_changed_cb_vec_t = std::vector<data_changed_cb>;

    void                        call_on_changed     (const data_path& path) const;

    data_path                   path_                   {};
    data_changed_cb_vec_t       data_changed_cb_vec_    {};
};


class data_source_invalid : public data_source_base
{
public:
    static data_source_invalid instance();
private:
    data_source_invalid() = default;
    ~data_source_invalid() override = default;

protected:
    const data_value&   do_as_data_value    (const data_path&) const override                   { return data_value_; }
    void                do_set              (const data_path&, data_value ) override            {};
    void                do_set_data_vec     (const data_path&, data_value_vec) override         {};
    void                do_set_data_source  (const data_path&, data_source_base_sp) override    {};
    std::string         do_to_string        () const override                                   { return "data_source_invalid";  }

    bool                do_is_read_only     () const override                                   { return true;      }
    bool                do_is_valid         () const override                                   { return false;     }
private:
    data_value          data_value_{};
};


} // namespace lue::data
