package cz.cvut.fit.tjv.realestates.controller;

import cz.cvut.fit.tjv.realestates.application.CustomerService;
import cz.cvut.fit.tjv.realestates.application.PropertyService;
import cz.cvut.fit.tjv.realestates.controller.converter.DTOConverter;
import cz.cvut.fit.tjv.realestates.controller.dto.CustomerDTO;
import cz.cvut.fit.tjv.realestates.domain.Customer;
import cz.cvut.fit.tjv.realestates.domain.Property;
import org.springframework.web.bind.annotation.*;

import java.time.LocalDate;
import java.util.List;

@RestController
@RequestMapping("/rest/api/customer")
public class CustomerController {
    private final CustomerService customerService;
    private final PropertyService propertyService;
    private final DTOConverter<CustomerDTO, Customer> customerDTOConverter;

    public CustomerController(CustomerService customerService,
                              PropertyService propertyService,
                              DTOConverter<CustomerDTO, Customer> customerConverter){
        this.customerService = customerService;
        this.propertyService = propertyService;
        this.customerDTOConverter = customerConverter;
    }

    @GetMapping
    public List<CustomerDTO> getCustomers() {
        return customerService.getAllEntities().stream().map(customerDTOConverter::toDTO).toList();
    }

    @GetMapping(path = "{id}")
    public CustomerDTO getCustomer(@PathVariable("id") String email) {
        return customerDTOConverter.toDTO(customerService.getEntityById(email));
    }

    @PostMapping
    public CustomerDTO createCustomer(@RequestBody CustomerDTO customerDTO) {
        return customerDTOConverter.toDTO(customerService.createEntity(customerDTOConverter.toEntity(customerDTO)));
    }

    @PutMapping(path = "{id}")
    public CustomerDTO updateCustomer(@PathVariable("id") String email, @RequestBody CustomerDTO customerDTO) {
        Customer customer = customerDTOConverter.toEntity(customerDTO);
        customer.setEmail(email);
        return customerDTOConverter.toDTO(customerService.updateEntity(customer));
    }

    @DeleteMapping(path = "date")
    public void deleteCustomersUnderAgeOfEighteen() {
        customerService.deleteCustomersBornAfter(LocalDate.now().minusYears(18));
    }

    @DeleteMapping(path = "{id}")
    public void deleteCustomer(@PathVariable("id") String email) {
        customerService.deleteEntity(email);
    }

}
