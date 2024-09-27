Đồ án cơ sở:
  Phân loại sản phẩm theo chiều cao. 
    Cơ cấu chấp hành: 
    + Khối cảm biến: Cảm biến tiệm cận loại NPN. Được phân hóa theo chiều cao của sản phẩm lần lượt 10cm, 7cm, 4cm. Cảm biến này sẽ phát ra một đường ánh sáng giống như tia hồng ngoại và khi vật thể chắn ngang chùm sáng đó, cảm biến sẽ phát hiện sự thay đổi cường độ ánh sáng và đưa ra tín hiệu điện về MCU để xử lý. 
    + Khối thực thi: Sử dụng servo SG90 để thực hiện hành dộng đã lập trình trong code. Mỗi servo tương ứng với từng vị trí của cảm biến, sao khi cảm biến phát hiện vật cản và gửi data về MCU để thực thi lệnh thì các servo này sẽ tiến hành quay với độ rộng xung 1ms tương ứng 90 độ. 
    + Khối nút nhấn: Sử dụng 3 nút nhấn cho các chức năng (BẮT ĐẦU, RESET, STOP). Đối với 2 nút nhấn BẮT ĐẦU và STOP chúng ta sẽ đấu nối theo cách vật lý, nút RESET chúng ta sẽ mắc qua 1 trở kéo xuống với giá trị 10K Ohm và tiến hành setup code phần MCLR lên mức ON. 
    + Khối MCU: Sử dụng PIC16F887 và trình biên dịch MPLAB IDE. Có nhiệm vụ xủ lý các data nhận được tù cảm biến và nút RESET khi data gửi về và điều khiển khối thực thi hoạt động và đồng thời lấy cái data đó đưa lên màn LCD theo từng hàng và từng cột xác định. 
    + Khối hiển thị: Sử dụng màn LCD 20x4, để hiển thị các giá trị như (Tên đồ án, Tên sản phẩm, Số lượng từn sản phẩm, Tổng cộng).
