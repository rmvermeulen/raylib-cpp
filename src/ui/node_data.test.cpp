#include "node_data.h"
#include "../functions.h"
#include "../testing.h"

namespace tests {

void node_data_test() {
    using namespace testing;
    describe("NodeData", [](const Context& $) {
        $.it("counts total nodes", [] {
            {
                ui::NodeData data{ui::NodeType::Row};
                expect(data.get_total_count() == 1, "must be 1 total");
            }
            {
                ui::NodeData data{ui::NodeType::Row,
                                  {ui::NodeData{ui::NodeType::Row}}};

                expect(data.get_total_count() == 2, "must be 2 total");
            }
            {
                ui::NodeData data{ui::NodeType::Row,
                                  {ui::NodeData{ui::NodeType::Row},
                                   ui::NodeData{ui::NodeType::Row}}};

                expect(data.get_total_count() == 3, "must be 3 total");
            }
            {
                ui::NodeData data{
                    ui::NodeType::Row,
                    {ui::NodeData{ui::NodeType::Column,
                                  {ui::NodeData{ui::NodeType::Row},
                                   ui::NodeData{ui::NodeType::Row}}},
                     ui::NodeData{ui::NodeType::Column,
                                  {ui::NodeData{ui::NodeType::Row},
                                   ui::NodeData{ui::NodeType::Row}}}}};

                expect(data.get_total_count() == 7, "must be 7 total");
            }
        });
    });
}

} // namespace tests