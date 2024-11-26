package cz.cvut.fit.tjv.realestates.controller;

import cz.cvut.fit.tjv.realestates.application.CustomerService;
import cz.cvut.fit.tjv.realestates.controller.converter.DTOConverter;
import cz.cvut.fit.tjv.realestates.controller.dto.CustomerDTO;
import cz.cvut.fit.tjv.realestates.controller.dto.PropertyDTO;
import cz.cvut.fit.tjv.realestates.domain.Customer;
import cz.cvut.fit.tjv.realestates.domain.Property;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.*;

import java.util.List;

@Controller
@RequestMapping("rest/api/customer")
public class CustomerController {
    private final CustomerService customerService;
    private final DTOConverter<CustomerDTO, Customer> customerDTOConverter;
    private final DTOConverter<PropertyDTO, Property> propertyDTOConverter;

    public CustomerController(CustomerService customerService, DTOConverter<CustomerDTO, Customer> customerConverter,
                              DTOConverter<PropertyDTO, Property> propertyConverter) {
        this.customerService = customerService;
        this.customerDTOConverter = customerConverter;
        this.propertyDTOConverter = propertyConverter;
    }

    @GetMapping
    public List<CustomerDTO> getCustomers() {
        return customerService.getAllEntities().stream().map(customerDTOConverter::toDTO).toList();
    }

    @GetMapping(path = "{id}")
    public CustomerDTO getCustomer(@PathVariable("id") String email) {
        return customerDTOConverter.toDTO(customerService.getEntityById(email));
    }

    @GetMapping(path="{id}/subleases")
    public List<PropertyDTO> getSubleases(@PathVariable("id") String email) {
        return customerService.getEntityById(email).getSubleases().stream().map(propertyDTOConverter::toDTO).toList();
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

    @DeleteMapping(path = "{id}")
    public void deleteCustomer(@PathVariable("id") String email) {
        customerService.deleteEntity(email);
    }

}
